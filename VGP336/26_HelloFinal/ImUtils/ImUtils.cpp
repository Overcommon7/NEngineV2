#include "pch.h"
#include "ImUtils.h"

bool ImUtils::SerializeInt(const string& name, int& value)
{
	return ImGui::DragInt(name.c_str(), &value);
}

bool ImUtils::SerializeVector3(const string& name, Vector3& vector, float speed)
{
	return ImGui::DragFloat3(name.c_str(), &vector.x, speed);
}

bool ImUtils::SerializeFloat(const string& name, float& num, float speed)
{
	return ImGui::DragFloat(name.c_str(), &num);
}

bool ImUtils::SerializeBool(const string& name, bool& value)
{
	return ImGui::Checkbox(name.c_str(), &value);
}

void ImUtils::DrawString(const string& name, const string& str)
{
	ImGui::Text("%s: %s", name.data(), str.c_str());
}

void ImUtils::DrawBool(const string& name, bool state)
{
	ImGui::Text("%s: %s", name.data(), state ? "true" : "false");
}

void ImUtils::DrawInt(const string& name, int value)
{
	ImGui::Text("%s: %i", name.data(), value);
}

void ImUtils::DrawFloat(const string& name, float value)
{
	ImGui::Text("%s: %f", name.data(), value);
}

void ImUtils::DrawVector3(const string& name, const Vector3& vector)
{
	ImGui::Text("%s: [%f, %f, %f]", name.data(), vector.x, vector.y, vector.z);
}
