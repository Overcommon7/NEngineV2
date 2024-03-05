#include "EditorState.h"
#include "StateNames.h"
#include "CustomFactory.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;


void EditorState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
    mWorld.SetCustomService(CustomFactory::CustomServiceMake);
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");

    auto ps = mWorld.GetService<PhysicsService>();
    if (ps) ps->SetEnabled(false); 
}

void EditorState::Terminate()
{
    mWorld.Terminate();
}

void EditorState::Render()
{
    mWorld.Render();
}

void EditorState::Update(float deltaTime)
{
    mWorld.Update(deltaTime);
}

void EditorState::DebugUI()
{
    ImGui::Begin("EditorState", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mWorld.EditorUI();

    if (ImGui::Button("Save##EditorState"))
    {
        mWorld.SaveLevel(mWorld.GetLevelPath());
    }
    ImGui::SameLine();
    if (ImGui::Button("Exit##EditorState"))
    {
        MainApp().ChangeState(State::GAME);
    }

    ImGui::End();
}


