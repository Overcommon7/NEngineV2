#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace NEngine;
using namespace NEngine::NMath;
using namespace NEngine::Physics;

namespace
{
    std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
    ASSERT(sPhysicsWorld == nullptr, "PhysicsWorld is already initialized");

    sPhysicsWorld = std::make_unique<PhysicsWorld>();
    sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
    if (sPhysicsWorld)
    {
        sPhysicsWorld->Terminate();
        sPhysicsWorld.reset();
    }
}

PhysicsWorld*PhysicsWorld::Get()
{
    ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld is not initialized");
    return sPhysicsWorld.get();
}

void PhysicsWorld::Initialize(const Settings& settings)
{
    mSettings = settings;
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
    mDynamicWorld->setGravity(settings.gravity);                                                             
    mDynamicWorld->setDebugDrawer(&mDebugDrawer);

    if constexpr (mUseSoftBody)
    {
        mSoftbodyWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
        mSoftbodyWorld->setGravity(settings.gravity);
        mSoftbodyWorld->setDebugDrawer(&mDebugDrawer);
    }
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDynamicWorld);
    if constexpr (mUseSoftBody)
    {
        SafeDelete(mSoftbodyWorld);
    }
        
    SafeDelete(mSolver);
    SafeDelete(mInterface);
    SafeDelete(mDispatcher);
    SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
    if (mEnabled)
    {
        mDynamicWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
        if constexpr (mUseSoftBody)
        {
            mSoftbodyWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
        }
    }
   
    for (auto object : mPhysicsObjects)
        object->Update();
}

void PhysicsWorld::DebugUI()
{
    if (!mEnabled) return;

    ImGui::Checkbox("RenderPhysics", &mRenderDebugUI);
    if (!mRenderDebugUI) return;

    int debugMode = mDebugDrawer.getDebugMode();
    bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
    if (ImGui::Checkbox("[DBG] drawWireFrame", &isEnabled))
    {
        debugMode = (isEnabled ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::btIDebugDraw::DBG_DrawWireframe);
    }

    isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
    if (ImGui::Checkbox("[DBG] drawAABB", &isEnabled))
    {
        debugMode = (isEnabled ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::btIDebugDraw::DBG_DrawAabb);
    }

    mDebugDrawer.setDebugMode(debugMode);
    mDynamicWorld->debugDrawWorld();
    if constexpr (mUseSoftBody)
    {
        mSoftbodyWorld->debugDrawWorld();
    }
   
    
}

void PhysicsWorld::RegisterPhysicsObject(PhysicsObject* object)
{
    if (std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), object) != mPhysicsObjects.end()) return;

     mPhysicsObjects.push_back(object);
     
     auto rigidbody = object->GetRigidbody();

     if (rigidbody != nullptr)
         mDynamicWorld->addRigidBody(rigidbody);

     if constexpr (mUseSoftBody)
     {
         auto softbody = object->GetSoftBody();
         if (softbody != nullptr)
             mSoftbodyWorld->addSoftBody(softbody);
     }
}

void PhysicsWorld::UnRegisterPhysicsObject(PhysicsObject* object)
{
    auto it = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), object);
    if (it != mPhysicsObjects.end())
        mPhysicsObjects.erase(it);

    auto rigidbody = object->GetRigidbody();

    if (rigidbody != nullptr)
        mDynamicWorld->removeRigidBody(rigidbody);

    if constexpr (mUseSoftBody)
    {
        auto softbody = object->GetSoftBody();
        if (softbody != nullptr)
            mSoftbodyWorld->removeSoftBody(softbody);
    } 
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicWorld == nullptr, "PhysicsWorld: terminate must be called");

}

btSoftBody* PhysicsWorld::CreateSoftbody(int nodeCount)
{
    btSoftBody* softBody = new btSoftBody(&mSoftbodyWorld->getWorldInfo(), nodeCount, nullptr, nullptr);
    return softBody;
}

void NEngine::Physics::PhysicsWorld::SetEnabled(bool enabled)
{
    mEnabled = true;
}

void NEngine::Physics::PhysicsWorld::SetGravity(NMath::Vector3 gravity)
{
    mSettings.gravity = gravity;
    if (mDynamicWorld) mDynamicWorld->setGravity(gravity);
    if constexpr (mUseSoftBody)
    {
        if (mSoftbodyWorld) mSoftbodyWorld->setGravity(gravity);
    }    
}

void NEngine::Physics::PhysicsWorld::SetSimulationSteps(int steps)
{
    mSettings.simulationSteps = steps;
}
void NEngine::Physics::PhysicsWorld::SetFiexedTimeStep(float timeStep)
{
    mSettings.fixedTimeStep = timeStep;
}
