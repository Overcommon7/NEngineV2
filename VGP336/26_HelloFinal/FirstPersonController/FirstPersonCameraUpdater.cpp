#include "pch.h"
#include "FirstPersonCameraUpdater.h"

#include "ImUtils/ImUtils.h"

void FirstPersonCameraUpdater::Update(Camera* camera, Values& values, NEngine::Transform* cameraTransform, NEngine::Transform* playerTransform, float deltaTime)
{
	const auto cameraPosition(playerTransform->position + values.offset);

	camera->SetPosition(cameraPosition);
	cameraTransform->position = cameraPosition;

	if (imGuiHovered)
		return;

	auto input = InputSystem::Get();
	if (input->IsMouseDown(MouseButton::LBUTTON))
		return;

	camera->Yaw(input->GetMouseMoveX() * values.sensitivity * deltaTime);
	camera->Pitch(input->GetMouseMoveY() * values.sensitivity * deltaTime);
	values.direction = camera->GetDirection();
}

void FirstPersonCameraUpdater::DebugUI(Camera* camera, Values& values)
{
	imGuiHovered = ImGui::IsAnyItemHovered();

	if (!ImGui::TreeNode("Camera"))
		return;

	ImUtils::SerializeFloat("Sensitivity", values.sensitivity, 0.05f);
	ImUtils::SerializeVector3("Offset", values.offset, 0.01f);
	if (ImUtils::SerializeFloat("FOV", values.fov))
		camera->SetFov(values.fov);

	ImUtils::DrawVector3("Direction", camera->GetDirection());
	

	ImGui::TreePop();

		
}
