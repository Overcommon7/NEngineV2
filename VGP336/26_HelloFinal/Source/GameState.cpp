#include "pch.h"
#include "GameState.h"
#include "CustomFactory.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
    mWorld.SetCustomService(CustomFactory::CustomServiceMake);
    TextureManager::Get()->SetRootDirectory(fs::current_path());
    mWorld.LoadLevel("Level/final_world.json");

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


