#include "pch.h"
#include "FirstPersonController.h"



void FirstPersonController::Initialize()
{
	mAnimatorComponent = mOwner->GetComponent<AnimatorComponent>();
	mRigidbodyComponent = mOwner->GetComponent<RigidbodyComponent>();
	mCameraComponent = mOwner->GetWorld().GetGameObject("MainCamera")->GetComponent<CameraComponent>();

	mCamera = &mCameraComponent->GetCamera();
	mRigidbody = &mRigidbodyComponent->GetRigidbody();
	mAnimator = &mAnimatorComponent->GetAnimator();

	mOwner->GetWorld().GetService<UpdateService>()->Register(this);
}

void FirstPersonController::Terminate()
{
	mOwner->GetWorld().GetService<UpdateService>()->Unregister(this);
}

void FirstPersonController::Update(float deltaTime)
{
	FirstPersonMovement::Update(mRigidbody, mMovementValues, mCamera, deltaTime);
}

void FirstPersonController::DebugUI()
{
	FirstPersonMovement::DebugUI(mRigidbody, mMovementValues);
}

void FirstPersonController::Deserialize(rapidjson::Value& value)
{
}

void FirstPersonController::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
}
