#include "Precompiled.h"
#include "Services/RenderService.h"

#include "GameWorld.h"
#include "Services/CameraService.h"

#include "Components/ModelComponent.h"
#include "Components/MeshComponent.h"
#include "Components/Transform.h"
#include "Components/AnimatorComponent.h"

void NEngine::RenderService::Initialize()
{
	mCameraService = GetGameWorld().GetService<CameraService>();

	mShadowEffect.Initialize();
	mShadowEffect.SetDirectionalLight(mDirectionalLight);

	mStandardEffect.Initialize("../../Assets/Shaders/Standard.fx");
	mStandardEffect.SetDirectionalLight(mDirectionalLight);
	mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
	mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
}

void NEngine::RenderService::Terminate()
{
	mStandardEffect.Terminate();
	mShadowEffect.Terminate();
}

void NEngine::RenderService::Update(float deltaTime)
{
	mFPS = 1.0f / deltaTime;
}

void NEngine::RenderService::Render()
{
	const Graphics::Camera& camera = mCameraService->GetMain();
	mStandardEffect.SetCamera(camera);

	
	for (auto& entry : mRenderEntries)
	{
		for (auto& renderObject : entry.renderGroup)
		{
			renderObject.transform = *entry.transform;
	    }
	}

	mShadowEffect.Begin();
	for (const auto& entry : mRenderEntries)
	{
		if (entry.castShadows)
			Graphics::DrawRenderGroup(mShadowEffect, entry.renderGroup);
	}
	mShadowEffect.End();

	mStandardEffect.Begin();
	for (const auto& entry : mRenderEntries)
	{
		Graphics::DrawRenderGroup(mStandardEffect, entry.renderGroup);
	}
	mStandardEffect.End();
}

void NEngine::RenderService::DebugUI()
{
	ImGui::Text("FPS: %i", int(mFPS));
	ImGui::Separator();

	if (ImGui::CollapsingHeader("Lighting##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::DragFloat3("Directional##Light", &mDirectionalLight.direction.x, 0.01f, -0.01f, 1.0f)){
			mDirectionalLight.direction = NMath::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
	}

	mShadowEffect.DebugUI();
	mStandardEffect.DebugUI();
}

void NEngine::RenderService::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("LightDirection"))
	{
		const auto& dir = value["LightDirection"].GetArray();
		for (int i = 0; i < 3; ++i)
		{
			mDirectionalLight.direction.v[i] = dir[i].GetFloat();
		}
		mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
	}
	if (value.HasMember("ColorAmbient"))
	{
		const auto& color = value["ColorAmbient"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();
		mDirectionalLight.ambient = { r, g, b, a };
	}
	if (value.HasMember("ColorDiffuse"))
	{
		const auto& color = value["ColorDiffuse"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();
		mDirectionalLight.diffuse= { r, g, b, a };
	}
	if (value.HasMember("ColorSpecular"))
	{
		const auto& color = value["ColorSpecular"].GetArray();
		const float r = color[0].GetFloat();
		const float g = color[1].GetFloat();
		const float b = color[2].GetFloat();
		const float a = color[3].GetFloat();
		mDirectionalLight.specular = { r, g, b, a };
	}
}

void NEngine::RenderService::Register(const ModelComponent* modelComponent)
{
	Entry& entry = mRenderEntries.emplace_back();
	const GameObject& gameObject = modelComponent->GetOwner();
	entry.modelComponent = modelComponent;
	entry.transform = gameObject.GetComponent<Transform>();
	entry.castShadows = modelComponent->CastShadow();

	const auto animatorComp = gameObject.GetComponent<AnimatorComponent>();
	const Graphics::Animator* animator = nullptr;

	if (animatorComp != nullptr)
	{
		animator = &animatorComp->GetAnimator();	
	}

	entry.renderGroup = Graphics::CreateRenderGroup(modelComponent->GetModelId(), animator);
}

void NEngine::RenderService::Unregister(const ModelComponent* modelComponent)
{
	std::erase_if(mRenderEntries, [&modelComponent](Entry& entry) {
		if (modelComponent != entry.modelComponent)
			return false;

		Graphics::CleanupRenderGroup(entry.renderGroup);
		return true;
		});
}

void NEngine::RenderService::Register(const MeshComponent* meshComponent)
{
	Entry& entry = mRenderEntries.emplace_back();
	const GameObject& gameObject = meshComponent->GetOwner();
	entry.meshComponent = meshComponent;
	entry.transform = gameObject.GetComponent<Transform>();
	entry.castShadows = meshComponent->CastShadow();
	entry.renderGroup = Graphics::CreateRenderGroup(meshComponent->GetModel());
}

void NEngine::RenderService::Unregister(const MeshComponent* meshComponent)
{
	std::erase_if(mRenderEntries, [&meshComponent](Entry& entry) {
		if (meshComponent != entry.meshComponent)
			return false;

		Graphics::CleanupRenderGroup(entry.renderGroup);
		return true;
		});
}
