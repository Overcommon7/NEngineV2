#pragma once

struct CustomFactory
{
    static bool CustomComponentMake(const string& componentName, rapidjson::Value& value, NEngine::GameObject& gameObject);
    static bool CustomServiceMake(const string& componentName, rapidjson::Value& value, NEngine::GameWorld& gameObject);
};


#define ComponentCount static_cast<uint32_t>(NEngine::ComponentId::Count)
#define ServiceCount static_cast<uint32_t>(NEngine::ServiceId::Count)

enum class CustomComponentId
{
	FirstPersonController = ComponentCount
};

enum class CustomServiceId
{
	NewService = ServiceCount
};

