#include "pch.h"
#include "FirstPersonController.h"

#include "ImUtils/ImUtils.h"
#include "FirstPersonAnimation.h"


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
	options.SetWeight(1.f, 3);
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
		{			
			FirstPersonMovement::Update(mTransform, mRigidbody, mCameraValues.direction, mMovementValues, deltaTime);
		}	
		else
		{
			OrbitCameraUpdate(deltaTime);
		}		
	}
	else
	{		
		FirstPersonMovement::Update(mTransform, mRigidbody, mCameraValues.direction, mMovementValues, deltaTime);
		FirstPersonCameraUpdater::Update(mCamera, mCameraValues, mCameraTransform, mTransform, deltaTime);
		//UpdateRotation();		
	}

	FirstPersonAnimation::Update(mAnimator, mRigidbody->GetRigidbody()->getLinearVelocity());

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

	if (ImGui::Button("Save"))
		mOwner->GetWorld().SaveTemplate(mOwner->GetTemplatePath(), mOwner->GetHandle());
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

	mValues.orbitCameraDirection = mCamera->GetDirection();
	mValues.orbitCameraPosition = mCamera->GetPosition();

}

void FirstPersonController::OnCameraSwitch()
{
	if (mValues.useOrbitCamera)
	{
		mCameraTransform->position = mValues.orbitCameraPosition;
		mCamera->SetPosition(mValues.orbitCameraPosition);
		mCamera->SetDirection(mValues.orbitCameraDirection);
	}
	else
	{
		mCamera->SetDirection(mCameraValues.direction);
	}
}

void FirstPersonController::UpdateRotation()
{
	auto ValidateFloat = [](float value) -> bool {
		return value != NAN && value != INFINITY && value != -INFINITY;
		};
	auto dir = mCamera->GetDirection();
	auto temp = dir;
	temp.y = 0;
	mCamera->SetDirection(temp);
	auto view = mCamera->GetViewMatrix();
	mCamera->SetDirection(dir);	
	auto rotation = Quaternion::CreateFromRotationMatrix(view * Matrix4::RotationY(btRadians(mValues.rotationOffset)));
	if (ValidateFloat(rotation.x) && ValidateFloat(rotation.y) && ValidateFloat(rotation.z) && ValidateFloat(rotation.w))
		mTransform->rotation = rotation;
}

void FirstPersonController::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("Speed"))
		mMovementValues.mSpeed = value["Speed"].GetFloat();
	if (value.HasMember("JumpForce"))
		mMovementValues.mJumpForce = value["JumpForce"].GetFloat();
	if (value.HasMember("LinearDamping"))
		mMovementValues.mLinearDamping = value["LinearDamping"].GetFloat();
	if (value.HasMember("AngularDamping"))
		mMovementValues.mAngularDamping = value["AngularDamping"].GetFloat();

	if (value.HasMember("RotationOffset"))
		mValues.rotationOffset = value["RotationOffset"].GetFloat();
	if (value.HasMember("OrbitCameraSpeed"))
		mValues.orbitCameraSpeed = value["OrbitCameraSpeed"].GetFloat();

	if (value.HasMember("Sensitivity"))
		mCameraValues.sensitivity = value["Sensitivity"].GetFloat();

	if (value.HasMember("Offset"))
	{
		const auto offset = value["Sensitivity"].GetArray();
		mCameraValues.offset.x = offset[0].GetFloat();
		mCameraValues.offset.y = offset[1].GetFloat();
		mCameraValues.offset.z = offset[2].GetFloat();
	}
		
}

void FirstPersonController::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value component(rapidjson::kObjectType);
	SaveUtil::SaveFloat("Speed", mMovementValues.mSpeed, doc, component);
	SaveUtil::SaveFloat("JumpForce", mMovementValues.mJumpForce, doc, component);
	SaveUtil::SaveFloat("LinearDamping", mMovementValues.mLinearDamping, doc, component);
	SaveUtil::SaveFloat("AngularDamping", mMovementValues.mAngularDamping, doc, component);

	SaveUtil::SaveFloat("RotationOffset", mValues.rotationOffset, doc, component);
	SaveUtil::SaveFloat("OrbitCameraSpeed", mValues.orbitCameraSpeed, doc, component);

	SaveUtil::SaveFloat("Sensitivity", mCameraValues.sensitivity, doc, component);
	SaveUtil::SaveVector3("Offset", mCameraValues.offset, doc, component);

	value.AddMember("FirstPersonController", component, doc.GetAllocator());
	
}


