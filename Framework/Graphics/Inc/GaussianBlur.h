#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "RenderTarget.h"
#include "VertexShader.h"

namespace NEngine::Graphics
{
	class RenderObject;
	class Texture;

	class GaussianBlur
	{
	public:
		void Initalize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);
		void DebugUI();

		inline void SetSourceTexture(const Texture& texture) { mSourceTexture = &texture; }

		inline const Texture& GetHorizantalBlurTexture() const { return mHorizantalBlurRenderTarget; }
		inline const Texture& GetVerticalBlurTexture() const { return mVerticalBlurRenderTarget; }
		inline const Texture& GetResultTexture() const { return mVerticalBlurRenderTarget; }

		inline uint32_t GetBlurIterations() { return mBlurIterations; }
		inline float GetBlurSaturation() { return mBlurSaturation; }

		inline void SetBlurIterations(uint32_t it) { mBlurIterations = it; }
		inline void SetBlurSaturation(float sat) { mBlurSaturation = sat; }

	private:

		struct SettingsData
		{
			float screenWidth;
			float screenHeight;
			float multiplier;
			float padding;
		};

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;

		RenderTarget mHorizantalBlurRenderTarget;
		RenderTarget mVerticalBlurRenderTarget;

		VertexShader mVertexShader;
		PixelShader mHorizontalBlurPixelShader;
		PixelShader mVerticalBlurPixelShader;

		Sampler mSampler;

		const Texture* mSourceTexture = nullptr;
		float mBlurSaturation = 1.0f;
		uint32_t mBlurIterations = 1;

	};
}

