#pragma once
#include "Component.h"

namespace NEngine
{
	class ColliderComponent	: public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Collider);

		void Initialize() override;
		void Terminate() override;
		void Deserialize(rapidjson::Value& value) override;

		friend class RigidbodyComponent;
	private:
		Physics::CollisionShape mCollisionShape;	
	};
}


