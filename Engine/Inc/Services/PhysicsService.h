#pragma once
#include "Service.h"
namespace NEngine
{
	class RigidbodyComponent;


	class PhysicsService : public Service
	{
	public:
		SET_TYPE_ID(ServiceId::Physics);
		
		void Update(float deltaTime) override;
		void DebugUI() override;

		void Deserialize(rapidjson::Value& value) override;

		void Register(RigidbodyComponent& rb);
		void Unregister(RigidbodyComponent& rb);
	};
}


