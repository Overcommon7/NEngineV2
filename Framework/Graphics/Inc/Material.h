#pragma once

#include "Colors.h"

namespace NEngine::Graphics
{
	struct Material
	{
		Color ambient =  Colors::White;
		Color diffuse =  Colors::White;
		Color specular = Colors::White;
		Color emissive = Colors::Black;
		float materialPower = 1.0f;
		float padding[3] = { 0.0f };
	};

}