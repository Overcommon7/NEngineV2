#pragma once

#include "Animation.h"

namespace NEngine::Graphics
{
	class AnimationClip
	{
	public:
		string name;
		float tickDuration = 0.f;
		float ticksPerSecond = 0.f;

		vector<std::unique_ptr<Animation>> boneAnimations;
	};
}
