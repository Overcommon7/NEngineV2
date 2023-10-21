#pragma once

#include "Common.h"

namespace NEngine::Graphics
{
	struct Bone
	{
		string name;
		int index = -1;

		Bone* parent = nullptr;
		int parentIndex = -1;

		vector<Bone*> children;
		vector<int> childrenIndicies;

		NMath::Matrix4 toParentTransform;
		NMath::Matrix4 offsetTransform;
	};
}
