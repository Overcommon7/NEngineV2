#include "EditTemplateState.h"
#include "StateNames.h"
#include "CustomFactory.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void EditTemplateState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomFactory::CustomComponentMake);
    mWorld.SetCustomService(CustomFactory::CustomServiceMake);
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");

    auto ps = mWorld.GetService<PhysicsService>();
    if (ps) ps->SetEnabled(false);        
}

void EditTemplateState::Terminate()
{
    mWorld.Terminate();
}

void EditTemplateState::Render()
{
    mWorld.Render();
}

void EditTemplateState::Update(float deltaTime)
{
    mWorld.Update(deltaTime);
}

void EditTemplateState::DebugUI()
{
    ImGui::Begin("Edit Template State", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mWorld.EditorUI();

    if (ImGui::Button("Save##EditTempalte"))
    {
        GameObject* object = mWorld.GetGameObject(GameWorld::GetEditObject());
        mWorld.SaveTemplate(object->GetTemplatePath(), object->GetHandle());
    }
    ImGui::SameLine();
    if (ImGui::Button("Reload##EditTempalte"))
    {
        MainApp().ChangeState(State::TEMPLATE);
    }
    ImGui::SameLine();
    if (ImGui::Button("Exit##Template"))
    {
        GameWorld::SetEditObject("");
        MainApp().ChangeState(State::EDITOR);
    }

    ImGui::End();
}


