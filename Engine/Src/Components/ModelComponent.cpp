#include "Precompiled.h"
#include "Components/ModelComponent.h"

#include "Services/RenderService.h"
#include "GameObject.h"
#include "GameWorld.h"

#include "SaveUtil.h"

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
	if (value.HasMember("CastShadow"))
	{
		mCastShadow = value["CastShadow"].GetBool();
	}
}

void NEngine::ModelComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	CMP_VALUE;
	
	SaveUtil::SaveString("FileName", mFileName.c_str(), doc, componentValue);
	SaveUtil::SaveStringArray("Animations", mAnimations, doc, componentValue);
	SaveUtil::SaveBool("CastShadow", mCastShadow, doc, componentValue);
	value.AddMember("ModelComponent", componentValue, doc.GetAllocator());
}
