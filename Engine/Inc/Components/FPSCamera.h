#pragma once
#include "CameraComponent.h"

namespace NEngine
{
	class FPSCamera	: public CameraComponent
	{
	public:
		void Update(float deltaTime) override;
		void Initialize() override;
		void Terminate() override;

		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(rapidjson::Value& value) override;
	private:
		float mMoveSpeed = 5.f;
		float mFastMoveSpeed = 25.f;
		float mTurnSpeed = 0.1f;
	};
}


