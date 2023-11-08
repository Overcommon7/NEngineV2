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
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDynamicWorld);
    SafeDelete(mSolver);
    SafeDelete(mInterface);
    SafeDelete(mDispatcher);
    SafeDelete(mCollisionConfiguration);
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
    for (auto object : mPhysicsObjects)
        object->Update();
}

void PhysicsWorld::DebugUI()
{
    ImGui::Checkbox("RenderPhysics", &mRenderDebugUI);
    if (!mRenderDebugUI) return;

    int debugMode = mDebugDrawer.getDebugMode();
    bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
    if (ImGui::Checkbox("[DBG] drawWireFrame", &isEnabled))
    {
        debugMode = (isEnabled ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::btIDebugDraw::DBG_DrawWireframe);
        isEnabled = (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
        if (ImGui::Checkbox("[DBG] drawAABB", &isEnabled))
        {
            debugMode = (isEnabled ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::btIDebugDraw::DBG_DrawAabb);
        }
    }

    mDebugDrawer.setDebugMode(debugMode);
    mDynamicWorld->debugDrawWorld();
    
}

void PhysicsWorld::RegisterPhysicsObject(PhysicsObject* object)
{
    if (std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), object) != mPhysicsObjects.end()) return;

     mPhysicsObjects.push_back(object);
     auto body = object->GetRigidBody();
     if (body != nullptr)
         mDynamicWorld->addRigidBody(body);

}

void PhysicsWorld::UnRegisterPhysicsObject(PhysicsObject* object)
{
    auto it = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), object);
    if (it != mPhysicsObjects.end())
        mPhysicsObjects.erase(it);
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicWorld == nullptr, "PhysicsWorld: terminate must be called");

}
