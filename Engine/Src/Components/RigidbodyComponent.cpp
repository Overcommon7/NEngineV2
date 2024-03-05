#include "Precompiled.h"
#include "Components/RigidbodyComponent.h"
#include "Components/Transform.h"
#include "Components/ColliderComponent.h"

#include "Services/PhysicsService.h"

#include "GameObject.h"
#include "GameWorld.h"

#include "SaveUtil.h"


namespace NEngine
{
	void RigidbodyComponent::Initialize()
	{
		ColliderComponent* collider = mOwner->GetComponent<ColliderComponent>();
		Transform* transform = mOwner->GetComponent<Transform>();
		ASSERT(collider != nullptr, "Requires Collider Component");

		mRigidbody.Initialize(*transform, collider->mCollisionShape, mMass);
		mOwner->GetWorld().GetService<PhysicsService>()->Register(*this);
	}

	void RigidbodyComponent::Terminate()
	{
		mOwner->GetWorld().GetService<PhysicsService>()->Unregister(*this);
		mRigidbody.Terminate();
	}

	void RigidbodyComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
	{
		rapidjson::Value componentValue(rapidjson::kObjectType);	
		SaveUtil::SaveFloat("Mass", mMass, doc, componentValue);
		value.AddMember("Rigidbody", componentValue, doc.GetAllocator());
		
	}

	void RigidbodyComponent::Deserialize(rapidjson::Value& value)
	{
		if (value.HasMember("Mass"))
		{
			mMass = value["Mass"].GetFloat();
	    }
	}

	void RigidbodyComponent::SetPosition(const NMath::Vector3& position)
	{
		mRigidbody.SetPosition(position);
	}
}

