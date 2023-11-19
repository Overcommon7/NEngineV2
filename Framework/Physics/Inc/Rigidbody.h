#pragma once

#include "PhysicsObject.h"

namespace NEngine::Graphics
{
	struct Transform;
}

namespace NEngine::Physics
{
	class CollisionShape;

	class Rigidbody	final : public PhysicsObject
	{
	public:
		Rigidbody() = default;
		~Rigidbody() override;

		void Initialize(NEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass = 0.0f);
		void Terminate();

		void SetPosition(const NEngine::NMath::Vector3& position);
		void SetVelocity(const NEngine::NMath::Vector3& velocity);

		bool IsDynamic() const;

		void Update() override;

		btRigidBody* GetRigidbody() const override {
			return mRigidbody;
		}
	private:
		

		btRigidBody* mRigidbody = nullptr;
		btDefaultMotionState* mMotionState = nullptr;
		float mass;

		NEngine::Graphics::Transform* mGraphicsTransform = nullptr;
	};
}



