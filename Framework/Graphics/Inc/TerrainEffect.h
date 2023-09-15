#pragma once

#include "ConstantBuffer.h"
#include "LightType.h"
#include "Material.h"
#include "PixelShader.h"
#include "Sampler.h"
#include "VertexShader.h"

namespace NEngine::Graphics
{
	class Camera;
	class Texture;
	class RenderObject;

	class TerrainEffect final
	{
	public:
		void Initialize();
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetLightCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void SetShadowMap(const Texture& shadowMap);

	private:
		struct TransformData
		{
			NMath::Matrix4 world;
			NMath::Matrix4 wvp;
			NMath::Matrix4 lwvp;
			NMath::Vector3 viewPosition;
			float padding = 0;
		};
		struct SettingsData
		{
			int useShadowMap = 0;
			float depthBias = 0.f;
			float padding[2] = { 0 };
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		using LightingBuffer = TypedConstantBuffer<DirectionalLight>;
		using MaterialBuffer = TypedConstantBuffer<Material>;
		using SettingBuffer = TypedConstantBuffer<SettingsData>;

		TransformBuffer mTransformBuffer;
		LightingBuffer mLightingBuffer;
		MaterialBuffer mMaterialBuffer;
		SettingBuffer mSettingBuffer;
		const Texture* shadowMap;

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		SettingsData mSettingsData;
		const Camera* mCamera = nullptr;
		const Camera* mLightCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		const Texture* mShadowMap = nullptr;
	};
}
