#pragma once
#include "Service.h"
namespace NEngine
{
	class CameraService;
	class ModelComponent;
	class Transform;
	class MeshComponent;

	class RenderService final : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Render);

		void Initialize() override;
		void Terminate() override;

		void Update(float deltaTime) override;
		void Render() override;
		void DebugUI() override;
		void Deserialize(rapidjson::Value& value) override;

		void Register(const ModelComponent* modelComponent);
		void Unregister(const ModelComponent* modelComponent);

		void Register(const MeshComponent* meshComponent);
		void Unregister(const MeshComponent* meshComponent);
	private:
		const CameraService* mCameraService = nullptr;

		Graphics::DirectionalLight mDirectionalLight;
		Graphics::StandardEffect mStandardEffect;
		Graphics::ShadowEffect mShadowEffect;

		float mFPS = 0.0f;

		struct Entry
		{
			bool castShadows = true;
			const ModelComponent* modelComponent = nullptr;
			const MeshComponent* meshComponent = nullptr;
			const Transform* transform = nullptr;
			Graphics::RenderGroup renderGroup;
		};

		using RenderEntities = vector<Entry>;
		RenderEntities mRenderEntries;
	};
}


