#pragma once

#include "Math/Inc/NMath.h"
#include "Core/Inc/Core.h"
#include "Graphics/Inc/Transform.h"


#include "Bullet/btBulletCollisionCommon.h"
#include "Bullet/btBulletDynamicsCommon.h"

template<class T>
inline void SafeDelete(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}