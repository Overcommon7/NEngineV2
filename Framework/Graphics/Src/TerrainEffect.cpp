#include "Precompiled.h"
#include "TerrainEffect.h"

#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"
#include "TextureManager.h"

using namespace NEngine::Graphics;
using namespace NEngine;

void TerrainEffect::Initialize()
{
	mTransformBuffer.Initialize();
	mLightingBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingBuffer.Initialize();

	string filepath = "../../Assets/Shaders/Terrain.fx";

	mVertexShader.Initialize<Vertex>(filepath);
	mPixelShader.Initialize(filepath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void TerrainEffect::Terminate()
{
	mSampler.Terminate();
	mSettingBuffer.Terminate();
	mPixelShader.Terminate();
	mMaterialBuffer.Terminate();
	mVertexShader.Terminate();
	mTransformBuffer.Terminate();
}

void TerrainEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightingBuffer.BindPS(1);
	mLightingBuffer.BindVS(1);
	
	mMaterialBuffer.BindPS(2);

	mSettingBuffer.BindPS(3);
	mSettingBuffer.BindVS(3);

	mSampler.BindPS(0);
	mSampler.BindVS(0);
}

void TerrainEffect::End()
{
	if (shadowMap != nullptr)
	{

	}
}

void TerrainEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "TerrainEffect: no camera specified");
	ASSERT(mDirectionalLight != nullptr, "TerrainEffect: no camera specified");

	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mCamera->GetViewMatrix();
	const auto& matProj = mCamera->GetProjectionMatrix();

	TransformData transformData;
	transformData.wvp = NMath::Transpose(matWorld * matView * matProj);
	mTransformBuffer.Update(transformData);

	if (mShadowMap && mLightCamera)
	{
		const auto& view = mLightCamera->GetViewMatrix();
		const auto& proj = mLightCamera->GetProjectionMatrix();
		transformData.lwvp = NMath::Transpose(matWorld * view * proj);
	}

	transformData.viewPosition = mCamera->GetPosition();

	SettingsData settings;
	settings.useShadowMap = (mSettingsData.useShadowMap > 0 && mShadowMap) ? 1 : 0;
	settings.depthBias = mSettingsData.depthBias;
	if (settings.useShadowMap)
		shadowMap->BindPS(2);


	mTransformBuffer.Update(transformData);
	mSettingBuffer.Update(settings);
	mMaterialBuffer.Update(renderObject.material);
	mLightingBuffer.Update(*mDirectionalLight);


	auto tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMapId, 1);

	renderObject.meshBuffer.Render();
}

void TerrainEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("TerrainEffect##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useShadows = mSettingsData.useShadowMap;
		if (ImGui::Checkbox("UseShadowMap##", &useShadows))
		{
			mSettingsData.useShadowMap = (useShadows) ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
	}
}

void TerrainEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void TerrainEffect::SetLightCamera(const Camera& camera)
{
	mLightCamera = &camera;
}

void TerrainEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}

void TerrainEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
}
