#include "CustomFactory.h"

bool CustomFactory::CustomComponentMake(const string& componentName, const rapidjson::Value& value, NEngine::GameObject& gameObject)
{
    if (componentName == "NewComponent")
    {
        return true;
    }
    return false;
}

bool CustomFactory::CustomServiceMake(const string& componentName, const rapidjson::Value& value, NEngine::GameWorld& gameObject)
{
    if (componentName == "NewService")
    {
        return true;
    }
    return false;
}
