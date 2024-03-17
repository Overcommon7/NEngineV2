#include "GameState.h"
#include "StateNames.h"
#include "CustomFactory.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
    mWorld.SetCustomService(CustomFactory::CustomServiceMake);
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");

    auto ps = mWorld.GetService<PhysicsService>();
    if (ps) ps->SetEnabled(true); 
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
    ImGui::Begin("GameState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mWorld.DebugUI();

    if (ImGui::Button("Edit##GameWrodl"))
    {
        MainApp().ChangeState(State::EDITOR);
    }

    ImGui::End();

    
}


