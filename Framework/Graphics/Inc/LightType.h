#pragma once

#include "Colors.h"

namespace NEngine::Graphics 
{
	struct DirectionalLight 
	{
		Color ambient = Colors::White;
		Color diffuse = Colors::White;
		Color specular = Colors::White;
		NMath::Vector3 direction = NMath::Vector3::ZAxis;
		float padding = 0;
	};

}