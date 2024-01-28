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

#include "Services/Service.h"
#include "Services/CameraService.h"
#include "Services/UpdateService.h"


#include "GameObjectFactory.h"

namespace NEngine
{
	App& MainApp();
}