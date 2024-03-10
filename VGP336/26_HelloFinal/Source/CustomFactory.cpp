#include "pch.h"
#include "CustomFactory.h"

#include "FirstPersonController/FirstPersonController.h"

bool CustomFactory::CustomComponentMake(const string& componentName, rapidjson::Value& value, NEngine::GameObject& gameObject)
{
    if (componentName == "FirstPersonController")
    {
        auto fpController = gameObject.AddComponent<FirstPersonController>();
        fpController->Deserialize(value);
        return true;
    }
    return false;
}

bool CustomFactory::CustomServiceMake(const string& componentName, rapidjson::Value& value, NEngine::GameWorld& gameObject)
{
    if (componentName == "NewService")
    {
        return true;
    }
    return false;
}
