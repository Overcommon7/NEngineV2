#include "GameState.h"

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

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomComponentMake);
    mWorld.SetCustomService(CustomServiceMake);
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


