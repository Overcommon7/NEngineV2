#include "Precompiled.h"
#include "Components/FPSCamera.h"

#include "GameWorld.h"
#include "GameObject.h"
#include "Services/UpdateService.h"
#include "Components/Transform.h"

using namespace NEngine::Input;

void NEngine::FPSCamera::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? mFastMoveSpeed : mMoveSpeed) * deltaTime;
    const float turnSpeed = mTurnSpeed * deltaTime;

    if (input->IsKeyDown(KeyCode::W)) {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S)) {
        mCamera.Walk(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::D)) {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A)) {
        mCamera.Strafe(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::E)) {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q)) {
        mCamera.Rise(-moveSpeed);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON)) {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }

    mOwner->GetComponent<NEngine::Transform>()->position = mCamera.GetPosition();
}

void NEngine::FPSCamera::Initialize()
{
	CameraComponent::Initialize();
	auto* updateService = GetOwner().GetWorld().GetService<UpdateService>();

	updateService->Register(this);
}

void NEngine::FPSCamera::Terminate()
{
	CameraComponent::Terminate();
	auto* updateService = GetOwner().GetWorld().GetService<UpdateService>();

	updateService->Unregister(this);
}

void NEngine::FPSCamera::Deserialize(rapidjson::Value& value)
{
    CameraComponent::Deserialize(value);

    if (value.HasMember("MoveSpeed"))
    {
        mMoveSpeed = value["MoveSpeed"].GetFloat();
    }

    if (value.HasMember("FastMoveSpeed"))
    {
        mFastMoveSpeed = value["FastMoveSpeed"].GetFloat();
    }

    if (value.HasMember("TurnSpeed"))
    {
        mTurnSpeed = value["TurnSpeed"].GetFloat();
    }
}
