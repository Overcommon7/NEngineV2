#pragma once

#include "Inc/AppState.h"
#include "Inc/NEngine.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Physics;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);
	void DebugUI();

protected:

	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;

	GameObject mGameObject;
};


