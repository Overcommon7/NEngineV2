#include "Precompiled.h"
#include "Animator.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void Animator::Initialize(ModelId id)
{
	mId = id;
	mIsLooping = false;
	mAnimationTick = 0.f;
	mClipIndex = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipIndex = clipIndex;
	mIsLooping = looping;
	mAnimationTick = 0.f;
}

void Animator::Update(float deltaTime)
{
	if (mClipIndex < 0)
		return;

	auto model = ModelManager::Get()->GetModel(mId);
	const auto& animClip = model->animationClips[mClipIndex];

	mAnimationTick += animClip.ticksPerSecond * deltaTime;
	if (mAnimationTick > animClip.tickDuration)
	{
		if (mIsLooping)
		{
			while (mAnimationTick >= animClip.tickDuration)
				mAnimationTick -= animClip.tickDuration;
		}
		else
		{
			mAnimationTick = animClip.tickDuration;
		}
	}	
}

bool Animator::IsFinished() const
{
	if (mIsLooping || mClipIndex < 0) return false;
	return mAnimationTick >= ModelManager::Get()->GetModel(mId)->animationClips[mClipIndex].tickDuration;
}

size_t Animator::GetAnimationCount() const
{
	return ModelManager::Get()->GetModel(mId)->animationClips.size();
}

NMath::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (mClipIndex < 0)
		return bone->toParentTransform;

	auto model = ModelManager::Get()->GetModel(mId);
	const auto& animClip = model->animationClips[mClipIndex];
	auto& animation = animClip.boneAnimations[bone->index];
	if (!animation)
		return bone->toParentTransform;

	return animation->GetTransform(mAnimationTick).GetMatrix4();
}
