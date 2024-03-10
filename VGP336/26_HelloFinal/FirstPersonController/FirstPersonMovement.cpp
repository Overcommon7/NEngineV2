#include "pch.h"
#include "FirstPersonMovement.h"

#include "ImUtils/ImUtils.h"

void FirstPersonMovement::Update(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime)
{
	UpdateInput(values.mControls);


	Vector3 movementVelocity = UpdateMovement(rigidbody, values, camera, deltaTime);
	Vector3 jumpVelocity = UpdateJump(rigidbody, values, camera, deltaTime);
	auto finalVelocity = movementVelocity + jumpVelocity;
	rigidbody->SetVelocity(finalVelocity);	
}

void FirstPersonMovement::DebugUI(Rigidbody* rigidbody, Values& values)
{
	if (!ImGui::TreeNode("Movement"))
		return;

	ImUtils::SerializeFloat("Speed: ", values.mSpeed);
	ImUtils::SerializeFloat("Jump Force: ", values.mJumpForce);

	Vector3 velocity(rigidbody->GetRigidbody()->getLinearVelocity());
	ImUtils::DrawVector3("Velocity", velocity);

	const string isDown(" Is Down");
	for (size_t i = 0; i < (size_t)BindType::Count; ++i)
	{
		auto& keybind = values.mControls.mKeyBinds[i];
		string name(magic_enum::enum_name((BindType)i).data());
		ImUtils::DrawBool(name + isDown, keybind.isDown);
	}
	ImGui::TreePop();
}

void FirstPersonMovement::UpdateInput(Values::Controls& controls)
{
	auto input = InputSystem::Get();
	for (size_t i = 0; i < (size_t)BindType::Count; ++i)
	{
		auto& keybind = controls.mKeyBinds[i];
		bool wasDownLastFrame = keybind.isDown;
		keybind.isDown = input->IsKeyDown(keybind.key);
		keybind.pressedThisFrame = input->IsKeyPressed(keybind.key);
		keybind.releasedThisFrame = wasDownLastFrame && !keybind.isDown;
	}
	
}

Vector3 FirstPersonMovement::UpdateMovement(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime)
{
	auto& controls = values.mControls;
	Vector3 velocity{};
	Vector3 rVelocity(rigidbody->GetRigidbody()->getLinearVelocity());
		

	if (controls[BindType::Backward].isDown)
	{
		velocity -= values.mSpeed * deltaTime * camera->GetDirection();
	}
	if (controls[BindType::Foward].isDown)
	{
		velocity += values.mSpeed * deltaTime * camera->GetDirection();
	}
	if (controls[BindType::Left].isDown)
	{
		const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, camera->GetDirection()));
		velocity.x -= values.mSpeed * deltaTime;
	}
	if (controls[BindType::Right].isDown)
	{
		const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, camera->GetDirection()));
		velocity.x += values.mSpeed * deltaTime;
	}

	return rVelocity.operator+(velocity);
}

Vector3 FirstPersonMovement::UpdateJump(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime)
{
	auto& controls = values.mControls;
	Vector3 velocity{};

	if (controls[BindType::Jump].pressedThisFrame)
	{
		velocity.y = values.mJumpForce;
	}

	return velocity;
}
