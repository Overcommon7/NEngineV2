#pragma once

#include <../Engine/Inc/NEngine.h>

struct CustomFactory
{
    static bool CustomComponentMake(const string& componentName, const rapidjson::Value& value, NEngine::GameObject& gameObject);
    static bool CustomServiceMake(const string& componentName, const rapidjson::Value& value, NEngine::GameWorld& gameObject);
};

