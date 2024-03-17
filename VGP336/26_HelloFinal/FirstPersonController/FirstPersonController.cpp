#include "pch.h"
#include "FirstPersonController.h"

#include "ImUtils/ImUtils.h"


void FirstPersonController::Initialize()
{
	mAnimatorComponent = mOwner->GetComponent<AnimatorComponent>();
	mRigidbodyComponent = mOwner->GetComponent<RigidbodyComponent>();


	auto camera = mOwner->GetWorld().GetGameObject("MainCamera");
	mCameraComponent = camera->GetComponent<CameraComponent>();
	mCameraTransform = camera->GetComponent<NEngine::Transform>();
	mCamera = &mCameraComponent->GetCamera();
	
	mRigidbody = &mRigidbodyComponent->GetRigidbody();
	mAnimator = &mAnimatorComponent->GetAnimator();
	mTransform = mOwner->GetComponent<NEngine::Transform>();


	auto& options = mAnimator->GetBlendOptions();
	const auto& clips = mAnimator->GetAnimationClips();
	int index = 0;
	for (const auto& clip : clips)
	{
		if (index != 0)
			options.AddClip(index, true, 0, 0);
		++index;
	}
	options.SetWeight(1.f, 0);
	mAnimator->SetBlending(true);
	

	mOwner->GetWorld().GetService<UpdateService>()->Register(this);

	mRigidbody->GetRigidbody()->setDamping(mMovementValues.mLinearDamping, mMovementValues.mAngularDamping);
}

void FirstPersonController::Terminate()
{
	mOwner->GetWorld().GetService<UpdateService>()->Unregister(this);

	mCameraComponent = nullptr;
	mAnimatorComponent = nullptr;
	mRigidbodyComponent = nullptr;

	mAnimator = nullptr;
	mRigidbody = nullptr;
	mCamera = nullptr;

	mTransform = nullptr;
	mCameraTransform = nullptr;
}

void FirstPersonController::Update(float deltaTime)
{
	auto input = InputSystem::Get();


	if (mValues.useOrbitCamera)
	{
		if (input->IsMouseDown(MouseButton::RBUTTON))
			FirstPersonMovement::Update(mTransform, mRigidbody, mMovementValues, mCameraValues.direction, deltaTime);
		else
			OrbitCameraUpdate(deltaTime);
		
	}
	else
	{		
		FirstPersonMovement::Update(mTransform, mRigidbody, mMovementValues, mCameraValues.direction, deltaTime);
		FirstPersonCameraUpdater::Update(mCamera, mCameraValues, mCameraTransform, mTransform, deltaTime);
		
	}
	
	if (!input->IsKeyPressed(KeyCode::LEFT))
		return;

	mValues.useOrbitCamera = !mValues.useOrbitCamera;
	OnCameraSwitch();
}

void FirstPersonController::DebugUI()
{
	imGuiHovered = ImGui::IsAnyItemHovered();

	if (ImUtils::SerializeBool("Use Orbit Camera", mValues.useOrbitCamera))
		OnCameraSwitch();

	FirstPersonMovement::DebugUI(mTransform, mRigidbody, mMovementValues);
	FirstPersonCameraUpdater::DebugUI(mCamera, mCameraValues);
}

void FirstPersonController::OrbitCameraUpdate(float deltaTime)
{
	if (imGuiHovered)
		return;

	auto input = InputSystem::Get();
	auto moveSpeed = mValues.orbitCameraSpeed * deltaTime;
	auto turnSpeed = mCameraValues.sensitivity * deltaTime;

	if (input->IsKeyDown(KeyCode::W)) {
		mCamera->Walk(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::S)) {
		mCamera->Walk(-moveSpeed);
	}

	if (input->IsKeyDown(KeyCode::D)) {
		mCamera->Strafe(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::A)) {
		mCamera->Strafe(-moveSpeed);
	}

	if (input->IsKeyDown(KeyCode::E)) {
		mCamera->Rise(moveSpeed);
	}
	else if (input->IsKeyDown(KeyCode::Q)) {
		mCamera->Rise(-moveSpeed);
	}

	mCamera->Yaw(input->GetMouseMoveX() * turnSpeed);
	mCamera->Pitch(input->GetMouseMoveY() * turnSpeed);



}

void FirstPersonController::OnCameraSwitch()
{
	if (mValues.useOrbitCamera)
	{
		mCameraTransform->position = mValues.orbitCameraPosition;
		mCamera->SetPosition(mValues.orbitCameraPosition);
		mCamera->SetLookAt(mValues.orbitCameraLookAt);
	}
	else
	{
		mCamera->SetLookAt(mCameraValues.direction);
	}
}

void FirstPersonController::Deserialize(rapidjson::Value& value)
{
}

void FirstPersonController::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
}


