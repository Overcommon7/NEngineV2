#include "EditTemplateState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

namespace
{
    bool CustomComponentMake(const string& componentName, const rapidjson::Value& value, GameObject& gameObject)
    {
        if (componentName == "NewComponent")
        {
            return true;
        }
        return false;
    }

    bool CustomServiceMake(const string& componentName, const rapidjson::Value& value, GameWorld& gameObject)
    {
        if (componentName == "NewService")
        {
            return true;
        }
        return false;
    }
}

void EditTemplateState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    mWorld.SetCustomService(CustomServiceMake);
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");
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
    mWorld.EditorUI();
    if (ImGui::Button("Exit##Template"))
    {
        MainApp().ChangeState("EditState");
    }
}


