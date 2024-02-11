#include "Precompiled.h"
#include "Services/PhysicsService.h"

#include "Components/RigidbodyComponent.h"

namespace
{
	NEngine::NMath::Vector3 GetVector3(const rapidjson::GenericArray<false, rapidjson::Value>& arr)
	{
		float x = arr[0].GetFloat();
		float y = arr[1].GetFloat();
		float z = arr[2].GetFloat();
		return Vector3(x, y, z);
	}
}


namespace NEngine
{
	using namespace Physics;


	void PhysicsService::Update(float deltaTime)
	{
		PhysicsWorld::Get()->Update(deltaTime);
	}

	void PhysicsService::DebugUI()
	{
		PhysicsWorld::Get()->DebugUI();
	}

	void PhysicsService::Deserialize(rapidjson::Value& value)
	{
		const auto& pw = PhysicsWorld::Get();
		if (value.HasMember("Gravity"))
		{
			const auto& gravity = value["Gravity"].GetArray();
			pw->SetGravity(GetVector3(gravity));
		}

		if (value.HasMember("SimSteps"))
		{
			const int simSteps = value["SimSteps"].GetInt();
			pw->SetSimulationSteps(simSteps);
		}

		if (value.HasMember("FixedTimeSteps"))
		{
			const float timeStep = value["FixedTimeSteps"].GetFloat();
			pw->SetFiexedTimeStep(timeStep);
		}
	}

	void PhysicsService::Register(RigidbodyComponent& rb)
	{
		PhysicsWorld::Get()->RegisterPhysicsObject(&rb.mRigidbody);
	}

	void PhysicsService::Unregister(RigidbodyComponent& rb)
	{
		PhysicsWorld::Get()-> UnRegisterPhysicsObject(&rb.mRigidbody);
	}

}
