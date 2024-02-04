#include "Precompiled.h"
#include "Services/CameraService.h"

#include "Components/CameraComponent.h"

const NEngine::Graphics::Camera& NEngine::CameraService::GetMain() const
{
	return mMainCamera->GetCamera();
}

NEngine::Graphics::Camera& NEngine::CameraService::GetMain()
{
	return mMainCamera->GetCamera();
}

void NEngine::CameraService::Deserialize(rapidjson::Value& value)
{
}

void NEngine::CameraService::SetMainCamera(uint32_t index)
{
	if (index >= 0 && index < mCameraEntries.size())
	{
		mMainCamera = mCameraEntries[index];
	}
}

void NEngine::CameraService::Register(CameraComponent* cameraComponent)
{
	mCameraEntries.push_back(cameraComponent);
	if (!mMainCamera)
		mMainCamera = cameraComponent;
}

void NEngine::CameraService::Unregister(CameraComponent* cameraComponent)
{
	auto it = std::find(mCameraEntries.begin(), mCameraEntries.end(), cameraComponent);
	if (it == mCameraEntries.end())	
		return;

	if (mMainCamera == cameraComponent)
	{  
		if (mCameraEntries.empty())
			mMainCamera = nullptr;
		else mMainCamera = mCameraEntries.front();
	}
			
	mCameraEntries.erase(it);			
}

void NEngine::CameraService::DebugUI()
{
	Graphics::SimpleDraw::Render(GetMain());
}
