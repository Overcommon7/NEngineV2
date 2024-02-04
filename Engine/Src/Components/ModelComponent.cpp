#include "Precompiled.h"
#include "Components/ModelComponent.h"

#include "Services/RenderService.h"
#include "GameObject.h"
#include "GameWorld.h"

void NEngine::ModelComponent::Initialize()
{
	auto* mm = Graphics::ModelManager::Get();
	mModelId = mm->LoadModel(mFileName);

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
}
