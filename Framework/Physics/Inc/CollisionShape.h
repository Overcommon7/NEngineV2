#pragma once

namespace NEngine::Physics
{
	class CollisionShape final
	{
	public:
		CollisionShape() = default;
		~CollisionShape();

		void InitializeSphere(float radius);
		void InitializeBox(const NEngine::NMath::Vector3& halfExtents);
		void InitializeHull(const NEngine::NMath::Vector3& halfExtents, const NEngine::NMath::Vector3& origin);
		void InitializeEmpty();

		void Terminate();
	private:
		btCollisionShape* mCollisionShape = nullptr;
		friend class Rigidbody;

		btCollisionShape* GetCollisionShape() const {
			return mCollisionShape;
		}
	};
}



