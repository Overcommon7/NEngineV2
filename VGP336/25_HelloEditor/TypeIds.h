#pragma once

#include <../Engine/Inc/TypeIds.h>

#define ComponentCount static_cast<uint32_t>(NEngine::ComponentId::Count)
#define ServiceCount static_cast<uint32_t>(NEngine::ServiceId::Count)

enum class CustomComponentId
{
	NewComponent = ComponentCount
};

enum class CustomServiceId
{
	NewService = ServiceCount
};
