#include "pch.h"
#include "FirstPersonMovement.h"

#include "ImUtils/ImUtils.h"

void FirstPersonMovement::Update(NEngine::Transform* transform, Rigidbody* rigidbody, Values& values, float deltaTime)
{
	UpdateInput(values.mControls);

	auto m = transform->GetMatrix4();
	const Vector3 look = { m._31,m._32,m._33 };
	Vector3 movementVelocity = UpdateMovement(rigidbody, values, look, deltaTime);

	Vector3 jumpVelocity = UpdateJump(rigidbody, values);
	auto finalVelocity = movementVelocity + jumpVelocity;
	rigidbody->SetVelocity(finalVelocity);


}

void FirstPersonMovement::DebugUI(NEngine::Transform* transform, Rigidbody* rigidbody, Values& values)
{
	if (!ImGui::TreeNode("Movement"))
		return;

	bool dampingChanged = false;

	ImUtils::SerializeFloat("Speed: ", values.mSpeed);
	ImUtils::SerializeFloat("Jump Force: ", values.mJumpForce);
	dampingChanged |= ImUtils::SerializeFloat("Linear Damping", values.mLinearDamping, 0.01f);
	dampingChanged |= ImUtils::SerializeFloat("Angular Damping", values.mAngularDamping, 0.01f);
	if (dampingChanged)
		rigidbody->GetRigidbody()->setDamping(values.mLinearDamping, values.mAngularDamping);

	Vector3 velocity(rigidbody->GetRigidbody()->getLinearVelocity());
	Vector3 euler{};
	ImUtils::DrawVector3("Velocity", velocity);
	btQuaternion(transform->rotation).getEulerZYX(euler.z, euler.y, euler.x);
	ImUtils::DrawVector3("Rotation", euler);

	const string isDown(" Is Down");
	for (size_t i = 0; i < (size_t)BindType::Count; ++i)
	{
		auto& keybind = values.mControls.mKeyBinds[i];
		string name(magic_enum::enum_name((BindType)i).data());
		ImUtils::DrawBool(name + isDown, keybind.isDown);
	}

	//for (size_t i = 0; i < (size_t)BindType::Count; ++i)
	//{
	//	string name(magic_enum::enum_name((BindType)i).data());
	//	if (ImGui::Combo(name.data(), ))
	//}
	
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

Vector3 FirstPersonMovement::UpdateMovement(Rigidbody* rigidbody, Values& values, Vector3 direction, float deltaTime)
{
	auto& controls = values.mControls;
	Vector3 velocity{};
	Vector3 rVelocity(rigidbody->GetRigidbody()->getLinearVelocity());
		

	if (controls[BindType::Backward].isDown)
	{
		velocity -= values.mSpeed * deltaTime * direction;
	}
	if (controls[BindType::Foward].isDown)
	{
		velocity += values.mSpeed * deltaTime * direction;
	}
	if (controls[BindType::Left].isDown)
	{
		const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, direction));
		velocity.x -= values.mSpeed * deltaTime;
	}
	if (controls[BindType::Right].isDown)
	{
		const NMath::Vector3 right = NMath::Normalize(Cross(NMath::Vector3::YAxis, direction));
		velocity.x += values.mSpeed * deltaTime;
	}
	
	return rVelocity.operator+(velocity);
}

Vector3 FirstPersonMovement::UpdateJump(Rigidbody* rigidbody, Values& values)
{
	auto& controls = values.mControls;
	Vector3 velocity{};

	if (controls[BindType::Jump].pressedThisFrame)
	{
		velocity.y = values.mJumpForce;
	}

	return velocity;
}
