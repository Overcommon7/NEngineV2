#pragma once

#include "Common.h"

namespace NEngine
{
	enum class ComponentId
	{
		Invalid = 0,
		Transform,
		Camera,
		Model,
		Mesh,

		Count
	};

	enum class ServiceId
	{
		Invalid = 0,
		Camera,
		Update,
		Render,

		Count
	};
}



#define SET_TYPE_ID(id)\
	static uint32_t StaticGetTypeId() { return static_cast<uint32_t>(id); }\
	uint32_t GetTypeId() const override { return StaticGetTypeId(); }\
