#pragma once
#include "Component.h"
namespace NEngine
{
	class RigidbodyComponent final : public Component
	{
	public:
		SET_TYPE_ID(ComponentId::Rigidbody);

		void Initialize() override;
		void Terminate() override;
		void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
		void Deserialize(rapidjson::Value& value) override;

		void SetPosition(const NMath::Vector3& position);

		const Physics::Rigidbody& GetRigidbody() const { return mRigidbody; }
		Physics::Rigidbody& GetRigidbody() { return mRigidbody; }
	private:
		friend class PhysicsService;
		Physics::Rigidbody mRigidbody;
		float mMass = 1.0f;
	};
}


