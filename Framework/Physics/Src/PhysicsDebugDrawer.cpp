#include "Precompiled.h"
#include "PhysicsDebugDrawer.h"

#include "Graphics/Inc/SimpleDraw.h"
#include "Graphics/Inc/Colors.h"

using namespace NEngine;
using namespace NEngine::NMath;
using namespace NEngine::Physics;
using namespace NEngine::Graphics;

void PhysicsDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	SimpleDraw::AddLine(from, to, Color(color.x(), color.y(), color.z(), 1));
}

void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
{

}

void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
{
	LOG("[Physics Warning] %s", warningString);
}

void PhysicsDebugDrawer::draw3dText(const btVector3& location, const char* textString)
{

}

void PhysicsDebugDrawer::setDebugMode(int debugMode)
{
	mDebugMode = debugMode;
}

int PhysicsDebugDrawer::getDebugMode() const
{
	return mDebugMode;
}
