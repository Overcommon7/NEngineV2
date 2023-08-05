#pragma once

#include "Inc/AppState.h"
#include "../Framework/Graphics/Inc/GraphicsSystem.h"
#include "Input/Inc/InputSystem.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate() {}
	void Update(float deltaTime);
	void Render() {}
	void DebugUI() {}
};
