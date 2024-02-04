#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");
}

void GameState::Terminate()
{
    mWorld.Terminate();
}

void GameState::Render()
{
    mWorld.Render();
}

void GameState::Update(float deltaTime)
{
    mWorld.Update(deltaTime);
}

void GameState::DebugUI()
{
    mWorld.DebugUI();
}


