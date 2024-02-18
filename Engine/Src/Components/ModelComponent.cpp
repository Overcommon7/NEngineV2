#include "Precompiled.h"
#include "Components/ModelComponent.h"

#include "Services/RenderService.h"
#include "GameObject.h"
#include "GameWorld.h"

void NEngine::ModelComponent::Initialize()
{
	auto* mm = Graphics::ModelManager::Get();
	mModelId = mm->GetModelId(mFileName);

	if (mm->GetModel(mModelId) == nullptr)
	{
		mModelId = mm->LoadModel(mFileName);
		for (const auto& filename : mAnimations)
		{
			mm->AddAnimation(mModelId, filename);
		}
	}

	auto renderService = mOwner->GetWorld().GetService<RenderService>();
	renderService->Register(this);


}

void NEngine::ModelComponent::Terminate()
{
	auto renderService = mOwner->GetWorld().GetService<RenderService>();
	renderService->Unregister(this);
}

void NEngine::ModelComponent::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("FileName"))
	{
		mFileName = value["FileName"].GetString();
	}
	if (value.HasMember("Animations"))
	{
		const auto& animations = value["Animations"].GetArray();
		for (auto& animation : animations)
		{
			mAnimations.emplace_back(animation.GetString());
		}
	}
}
