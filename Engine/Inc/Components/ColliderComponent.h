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
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(rapidjson::Value& value) override;

		friend class RigidbodyComponent;
	private:
		Physics::CollisionShape mCollisionShape;

		struct LoadingData
		{
			string shapeType;
			NMath::Vector3 param0;
			NMath::Vector3 param1;
		};

		LoadingData mLoadingData;
	};
}


