#include "EditorState.h"

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

void EditorState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    mWorld.SetCustomService(CustomServiceMake);
    mWorld.LoadLevel("../../Assets/Templates/test_level.json");
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
    mWorld.EditorUI();
}


