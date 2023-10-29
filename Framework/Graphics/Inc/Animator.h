#pragma once

#include "ModelManager.h"
#include "Bone.h"

namespace NEngine::Graphics
{
	class Animator
	{
	public:
		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);

		bool IsFinished() const;
		size_t GetAnimationCount() const;
		NMath::Matrix4 GetToParentTransform(const Bone* bone) const;
	private:
		ModelId mId = 0;
		float mClipIndex = -1;
		float mAnimationTick = 0.f;
		bool mIsLooping = false;
	};
}
