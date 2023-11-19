#include "Precompiled.h"
#include "CollisionShape.h"

using namespace NEngine::Physics;

CollisionShape::~CollisionShape()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: Must Call Terminate!");
}

void CollisionShape::InitializeSphere(float radius)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: Must Call Terminate!");
	mCollisionShape = new btSphereShape(radius);
}

void CollisionShape::InitializeBox(const NEngine::NMath::Vector3& halfExtents)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: Must Call Terminate!");
	mCollisionShape = new btBoxShape(halfExtents);
}

void NEngine::Physics::CollisionShape::InitializeHull(const NEngine::NMath::Vector3& halfExtents, const NEngine::NMath::Vector3& origin)
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: Must Call Terminate!");
	btConvexHullShape* hullShape = new btConvexHullShape();

	std::vector<NEngine::NMath::Vector3> points =
	{
		{ -halfExtents.x, -halfExtents.y, -halfExtents.z},
		{ -halfExtents.x,  halfExtents.y, -halfExtents.z},
		{  halfExtents.x,  halfExtents.y, -halfExtents.z},
		{  halfExtents.x, -halfExtents.y, -halfExtents.z},
		{ -halfExtents.x, -halfExtents.y,  halfExtents.z},
		{ -halfExtents.x,  halfExtents.y,  halfExtents.z},
		{  halfExtents.x,  halfExtents.y,  halfExtents.z},
		{  halfExtents.x, -halfExtents.y,  halfExtents.z},
	};

	for (auto& point : points)
	{
		hullShape->addPoint(point + origin, false);
	}
	hullShape->recalcLocalAabb();
	mCollisionShape = hullShape;
}

void NEngine::Physics::CollisionShape::InitializeEmpty()
{
	ASSERT(mCollisionShape == nullptr, "CollisionShape: Must Call Terminate!");
	mCollisionShape = new btEmptyShape();
}

void CollisionShape::Terminate()
{
	ASSERT(mCollisionShape != nullptr, "CollisionShape: Must Call Initialize!");

	SafeDelete(mCollisionShape);
}
