#pragma once
#include "PhysicsDebugDrawer.h"

namespace NEngine::Physics
{
	class PhysicsObject;
	class PhysicsWorld final
	{
	public:
		struct Settings
		{
			NEngine::NMath::Vector3 gravity{ 0.0f, -9.81f, 0.0f };
			uint32_t simulationSteps = 1;
			float fixedTimeStep = 1.f / 60.f;
		};

		static void StaticInitialize(const Settings& settings);
		static void StaticTerminate();
		static PhysicsWorld* Get();

		void Initialize(const Settings& settings);
		void Terminate();

		void Update(float deltaTime);
		void DebugUI();

		void RegisterPhysicsObject(PhysicsObject* object);
		void UnRegisterPhysicsObject(PhysicsObject* object);

		PhysicsWorld() = default;
		~PhysicsWorld();
	private:

		Settings mSettings;
		bool mRenderDebugUI = false;

		PhysicsDebugDrawer mDebugDrawer;

		btBroadphaseInterface* mInterface = nullptr;
		btCollisionDispatcher* mDispatcher = nullptr;
		btSequentialImpulseConstraintSolver* mSolver = nullptr;
		btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
		btDiscreteDynamicsWorld* mDynamicWorld = nullptr;
		btSequentialImpulseConstraintSolver* mImpulseSolver = nullptr;

		using PhysicsObjects = vector<PhysicsObject*>;
		PhysicsObjects mPhysicsObjects;

		btSoftRigidDynamicsWorld* mSoftbodyWorld = nullptr;
		friend class SoftBody;

		btSoftBody* CreateSoftbody(int nodeCount);
	};
}

