#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace NEngine::Graphics
{
	class RenderObject;
	class Texture;

	class PostProcessingEffect
	{
	public:
		enum class Mode
		{
			 None,
			 Monochrome,
			 Invert,
			 Mirror,
			 Blur,
			 Combine2,
			 CRT
		};

		void Initialize(const std::filesystem::path& filepath);
		void Terminate();


		void Begin();
		void End();
		void Render(const RenderObject& renderObject);

		void SetTexture(const Texture* texture, uint32_t slot = 0);
		inline void SetMode(Mode mode) { mMode = mode; }

		void DebugUI();
		 

	private:

		struct PostPrcocessData
		{
			int mode = 0;
			float params0 = 0;
			float params1 = 0;
			float params2 = 0;
			float params3 = 0;
			NMath::Vector2 screenSize = {};
			NMath::Vector2 scanlineSize = {};
			float curvatureAmount = 0.05f;
			float colorBleed = 0.1f;
			float noiseIntensity = 0.05f;
		};

		struct Params
		{
			NMath::Vector2 mirrorScale = { -1, -1 };
			float blurStrength = 5;
			NMath::Vector2 scanlineSize = { 1, 2.9f };
			float darken = 0.3f;
			bool useBilinear = false;
			bool useNoise = true;
			bool useBleeding = true;
			float curvatureAmount = 0.09f;
			float colorBleed = 0.1f;
			float noiseIntensity = 0.05f;
		};

		using PostPrcocessBuffer = TypedConstantBuffer<PostPrcocessData>;
		PostPrcocessBuffer mPostProcessBuffer;

		Mode mMode = Mode::CRT;
		Params params;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		Sampler mLinearSampler;
		std::array<const Texture*, 4> mTextures;
	};
}
