#pragma once

#include "Inc/AppState.h"

using namespace NEngine;

class GameState : public AppState
{
public:
	void Initialize() {}
	void Terminate() {}
	void Update(float deltaTime) {}
	void Render() {}
	void DebugUI() {}
};
