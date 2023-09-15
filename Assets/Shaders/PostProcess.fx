cbuffer PostProcessBuffer : register(b0)
{
    int mode;
    float params0;
    float params1;
    float params2;
    float params3;
    float2 screenSize;
    float2 scanlineSize;
    float curvatureAmount;
    float colorBleedAmount;
    float noiseIntensity;
};


Texture2D textureMap0 : register(t0);
Texture2D textureMap1 : register(t1);
Texture2D textureMap2 : register(t2);
Texture2D textureMap3 : register(t3);

SamplerState textureSampler : register(s0);
SamplerState linearSampler : register(s1);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 position : SV_Position;
    float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    output.position = float4(input.position, 1.0f);
    output.texCoord = input.texCoord;
    return output;
}

float3 rand3(float3 seed)
{
    return frac(sin(seed * 0.9898) * 43758.5453);
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float4 finalColor = float4(0, 0, 0, 0);
    
    if (mode == 0)
    {
        finalColor = textureMap0.Sample(textureSampler, input.texCoord);
    }
    else if (mode == 1)
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = (color.r + color.g + color.b) / 3.0f;
    }
    else if (mode == 2)
    {
        float4 color = textureMap0.Sample(textureSampler, input.texCoord);
        finalColor = 1.0f - color;
    }
    else if (mode == 3)
    {
        float2 texCoord = input.texCoord;
        texCoord.x *= params0;
        texCoord.y *= params1;
        finalColor = textureMap0.Sample(textureSampler, texCoord);
    }
    else if (mode == 4)
    {
        float u = input.texCoord.x;
        float v = input.texCoord.y;
        
        finalColor =
            textureMap0.Sample(textureSampler, float2(u, v)) +
            textureMap0.Sample(textureSampler, float2(u + params0, v)) +
            textureMap0.Sample(textureSampler, float2(u - params0, v)) +
            textureMap0.Sample(textureSampler, float2(u, v + params1)) +
            textureMap0.Sample(textureSampler, float2(u, v - params1)) +
            textureMap0.Sample(textureSampler, float2(u + params0, v + params1)) +
            textureMap0.Sample(textureSampler, float2(u + params0, v - params1)) +
            textureMap0.Sample(textureSampler, float2(u - params0, v + params1));
            textureMap0.Sample(textureSampler, float2(u - params0, v - params1));
        
        finalColor *= 0.12f;
    }
    else if (mode == 5)
    {
        float4 color0 = textureMap0.Sample(textureSampler, input.texCoord);
        float4 color1 = textureMap1.Sample(textureSampler, input.texCoord);
        finalColor = (color0 + color1) * 0.5f;
    }
    else if (mode == 6)
    {
        float2 screenCoord = input.texCoord * screenSize;

        // Calculate the scanline index
        int scanlineIndex = (int) floor(screenCoord.y / scanlineSize.y);

        //Apply Scanlines
        if (params1 > 0)
        {
            finalColor = textureMap0.Sample(linearSampler, input.texCoord);
        }
        else
        {
            finalColor = textureMap0.Sample(textureSampler, input.texCoord);           
        }
            
        if (scanlineIndex % 2 == 0)
        {
            finalColor.rgb *= params0;
        }
        
        //Apply curvature effect
        float2 curvatureOffset = curvatureAmount * normalize(screenCoord - screenSize / 2);
        float2 curvedTexCoord = input.texCoord + curvatureOffset;
        
        //Apply color bleeding effect
        if (params2 > 0)
        {
            float4 bleedColor = 0;
            if (params1 > 0)
            {
                bleedColor = textureMap0.Sample(linearSampler, curvedTexCoord);
            }
            else
            {
                bleedColor = textureMap0.Sample(textureSampler, curvedTexCoord);
            }
            finalColor.rgb = lerp(finalColor.rgb, bleedColor.rgb, colorBleedAmount);
        }
 
        //Apply Noise
        if (params3 > 0)
        {
            float3 noiseSeed = float3(input.texCoord.x, input.texCoord.y, 0.0);
            float3 noise = rand3(noiseSeed);
            noise = noise * 2.0f - 1.0f;
            finalColor.rgb += noise * noiseIntensity;
        }
        
    }

    return finalColor;
}