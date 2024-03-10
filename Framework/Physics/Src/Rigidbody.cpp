#include "Precompiled.h"
#include "Rigidbody.h"

#include "CollisionShape.h"
#include <Graphics/Inc/Transform.h>

using namespace NEngine;
using namespace NEngine::Physics;
using namespace NEngine::Graphics;

Rigidbody::~Rigidbody()
{
    ASSERT(mRigidbody == nullptr, "Rigidbody: Must Call Terminate!");
}

void Rigidbody::Initialize(NEngine::Graphics::Transform& graphicsTransform, const CollisionShape& shape, float mass)
{
    mGraphicsTransform = &graphicsTransform;
    this->mass = mass;

    mMotionState = new btDefaultMotionState(ConvertTobtTransform(graphicsTransform));
    mRigidbody = new btRigidBody(mass, mMotionState, shape.mCollisionShape);
}

void Rigidbody::Terminate()
{
    SafeDelete(mMotionState);
    SafeDelete(mRigidbody);
    mRigidbody = nullptr;
}

void Rigidbody::SetPosition(const NEngine::NMath::Vector3& position)
{
    mGraphicsTransform->position = position;
    mRigidbody->setWorldTransform(ConvertTobtTransform(*mGraphicsTransform));
}

void Rigidbody::SetVelocity(const NEngine::NMath::Vector3& velocity)
{
    if (IsDynamic())
    {
        mRigidbody->activate();
    }

    mRigidbody->setLinearVelocity(velocity);
}

bool Rigidbody::IsDynamic() const
{
    return mass > 0.f;
}

void Rigidbody::Update()
{
    auto& worldTransform = mRigidbody->getWorldTransform();
    mGraphicsTransform->position = worldTransform.getOrigin();
    mGraphicsTransform->rotation = worldTransform.getRotation();
}
