#include "Precompiled.h"
#include "Components/Transform.h"

#include "SaveUtil.h"

void NEngine::Transform::DebugUI()
{
	Graphics::SimpleDraw::AddTransform(GetMatrix4());
}

void NEngine::Transform::Deserialize(rapidjson::Value& value)
{
	if (value.HasMember("Position"))
	{
		auto array = value["Position"].GetArray();
		for (int i = 0; i < array.Size(); ++i)
		{
			position.v[i] = array[i].GetFloat();
		}
	}
	if (value.HasMember("Rotation"))
	{
		auto array = value["Rotation"].GetArray();
		rotation.x = array[0].GetFloat();
		rotation.y = array[1].GetFloat();
		rotation.z = array[2].GetFloat();
		rotation.w = array[3].GetFloat();
	}
	if (value.HasMember("Scale"))
	{
		auto array = value["Scale"].GetArray();
		for (int i = 0; i < array.Size(); ++i)
		{
			scale.v[i] = array[i].GetFloat();
		}
	}
}

void NEngine::Transform::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
{
	rapidjson::Value componentValue(rapidjson::kObjectType);
	SaveUtil::SaveVector3("Position", position, doc, componentValue);
	SaveUtil::SaveQuaternion("Rotation", rotation, doc, componentValue);
	SaveUtil::SaveVector3("Scale", scale, doc, componentValue);
	value.AddMember("Transform", componentValue, doc.GetAllocator());
	

}
