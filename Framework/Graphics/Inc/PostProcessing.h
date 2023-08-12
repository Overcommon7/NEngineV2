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
			 MotionBlur
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
			float params0;
			float params1;
			float params2;
		};

		struct Params
		{
			NMath::Vector2 mirrorScale = { -1, -1 };
			float blurStrength = 5;
		};

		using PostPrcocessBuffer = TypedConstantBuffer<PostPrcocessData>;
		PostPrcocessBuffer mPostProcessBuffer;

		Mode mMode = Mode::None;
		Params params;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;
		std::array<const Texture*, 4> mTextures;
	};
}
