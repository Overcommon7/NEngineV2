#include "Precompiled.h"
#include "Components/CameraComponent.h"

#include "Services/CameraService.h"
#include "GameWorld.h"
#include "GameObject.h"

#include "SaveUtil.h"

void NEngine::CameraComponent::Initialize()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Register(this);
}

void NEngine::CameraComponent::Terminate()
{
	CameraService* cameraService = GetOwner().GetWorld().GetService<CameraService>();
	cameraService->Unregister(this);
}

void NEngine::CameraComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", mStartingPosition, doc, componentValue);
	SaveUtil::SaveVector3("LookAt", mStartingLookAt, doc, componentValue);
	value.AddMember("CameraComponent", componentValue, doc.GetAllocator());
}

void NEngine::CameraComponent::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& position = value["Position"].GetArray();
		if (position.Size() >= 3)
		{
			mStartingPosition.x = position[0].GetFloat();
			mStartingPosition.y = position[1].GetFloat();
			mStartingPosition.z = position[2].GetFloat();
			mCamera.SetPosition(mStartingPosition);
		}
			
	}
	
	if (value.HasMember("LookAt"))
	{
		auto lookAt = value["LookAt"].GetArray();
		if (lookAt.Size() >= 3)
		{
			mStartingLookAt.x = lookAt[0].GetFloat();
			mStartingLookAt.y = lookAt[1].GetFloat();
			mStartingLookAt.z = lookAt[2].GetFloat();
			mCamera.SetLookAt(mStartingLookAt);
		}
			
	}
}
