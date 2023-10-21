#pragma once

#include "Bone.h"

namespace NEngine::Graphics
{
	struct Skeleton
	{
		Bone* root = nullptr;
		vector<std::unique_ptr<Bone>> bones;

	};
}
