#pragma once
#include "Component.h"
namespace NEngine
{
	class Transform final : public Component, public Graphics::Transform
	{
	public:
		void DebugUI() override;
		virtual void Deserialize(rapidjson::Value& value) override;
		virtual void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		SET_TYPE_ID(ComponentId::Transform);


	};
}


