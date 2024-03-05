#pragma once

#include "Common.h"
#include "App.h"
#include "AppState.h"

#include "GameObject.h"
#include "Component.h"
#include "GameWorld.h"

#include "Components/Transform.h"
#include "Components/CameraComponent.h"
#include "Components/FPSCamera.h"
#include "Components/ModelComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ColliderComponent.h"
#include "Components/RigidbodyComponent.h"
#include "Components/SoundEffectComponent.h"
#include "Components/SoundBankComponent.h"
#include "Components/AnimatorComponent.h"

#include "Services/Service.h"
#include "Services/CameraService.h"
#include "Services/UpdateService.h"
#include "Services/PhysicsService.h"


#include "GameObjectFactory.h"

namespace NEngine
{
	App& MainApp();
}