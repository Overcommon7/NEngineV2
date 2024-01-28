#include "Precompiled.h"
#include "Components/CameraComponent.h"

#include "Services/CameraService.h"
#include "GameWorld.h"
#include "GameObject.h"

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

void NEngine::CameraComponent::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		const auto& position = value["Position"].GetArray();
		if (position.Size() >= 3)
		{
			float x = position[0].GetFloat();
			float y = position[1].GetFloat();
			float z = position[2].GetFloat();
			mCamera.SetPosition({x, y, z});
		}
			
	}
	
	if (value.HasMember("LookAt"))
	{
		auto lookAt = value["LookAt"].GetArray();
		if (lookAt.Size() >= 3)
			mCamera.SetLookAt({ lookAt[0].GetFloat(), lookAt[1].GetFloat(), lookAt[2].GetFloat() });
	}
}
