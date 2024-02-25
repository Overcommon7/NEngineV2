#pragma once

#include "Camera.h"
#include "ConstantBuffer.h"
#include "LightType.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace NEngine::Graphics
{
	class RenderObject;
	class ShadowEffect
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();
		inline void SetDirectionalLight(const DirectionalLight& directionalLight) { mDirectionalLight = &directionalLight; }
		inline void SetFocus(const NMath::Vector3& focusPosition) { mFocusPosition = focusPosition; }
		inline void SetSize(float size) { mSize = size; }
		inline const Camera& GetLightCamera() const { return mLightCamera; }
		inline const Texture& GetDepthMap() const { return mDepthMapRenderTarget; }
	private:

		void UpadateLightCamera();

		struct TransformData
		{
			NMath::Matrix4 wvp;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using BoneTransformBuffer = ConstantBuffer;

		TransformBuffer mTrasformBuffer;
		BoneTransformBuffer mBoneTransformBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const DirectionalLight* mDirectionalLight = nullptr;

		Camera mLightCamera;
		RenderTarget mDepthMapRenderTarget;

		NMath::Vector3 mFocusPosition = NMath::Vector3::Zero;
		float mSize = 100.f;
	};
}
