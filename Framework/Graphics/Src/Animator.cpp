#include "Precompiled.h"
#include "Animator.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void Animator::Initialize(ModelId id)
{
	mId = id;
	mClipData.isLooping = false;
	mClipData.currentTick = 0.f;
	mClipData.index = -1;
}

void Animator::PlayAnimation(int clipIndex, bool looping)
{
	mClipData.index = clipIndex;
	mClipData.StartAnimation(looping);
}

void Animator::Update(float deltaTime)
{
	auto model = ModelManager::Get()->GetModel(mId);

	if (mIsBlending)
	{
		UpdateWithBlend(deltaTime, model);
	}
	else
	{
		UpdateNoBlend(deltaTime, mClipData, model);
	}
	
}


void NEngine::Graphics::Animator::UpdateNoBlend(float deltaTime, ClipData& clipData, const Model* model)
{
	if (clipData.index < 0)
		return;

	const auto& animClip = model->animationClips[clipData.index];

	clipData.currentTick += animClip.ticksPerSecond * deltaTime;
	if (clipData.currentTick > animClip.tickDuration)
	{
		if (clipData.isLooping)
		{
			while (clipData.currentTick >= animClip.tickDuration)
				clipData.currentTick -= animClip.tickDuration;
		}
		else
		{
			clipData.currentTick = animClip.tickDuration;
		}
	}
}

void NEngine::Graphics::Animator::UpdateWithBlend(float deltaTime, const Model* model)
{
	for (auto& data : mBlendOptions.clips)
	{
		UpdateNoBlend(deltaTime, data, model);
	}
}

void NEngine::Graphics::Animator::SetBlending(bool blend)
{
	mIsBlending = blend && BlendOptions::CLIP_COUNT > 1;
}

bool Animator::IsFinished(int clipIndex) const
{
	auto check = [](const ClipData& data, size_t id)
	{
		if (data.isLooping || data.index < 0) return false;

		return data.currentTick >= ModelManager::Get()->GetModel(id)->
			animationClips[data.index].tickDuration;
	};

	if (!mIsBlending || clipIndex == -1)
		return check(mClipData, mId);

	return check(mBlendOptions.clips[clipIndex], mId);
	
}

bool NEngine::Graphics::Animator::IsBlending() const
{
	return mIsBlending;
}

size_t Animator::GetAnimationCount() const
{
	return ModelManager::Get()->GetModel(mId)->animationClips.size();
}

NMath::Matrix4 Animator::GetToParentTransform(const Bone* bone) const
{
	if (!mIsBlending && mClipData.index < 0)
		return bone->toParentTransform;

	if (mIsBlending)
	{
		size_t size = GetAnimationCount();
		for (auto& clip : mBlendOptions.clips)
			if (clip.index < 0 || clip.index >= size)
				return bone->toParentTransform;

		return GetToParentTransformWithBlend(bone);
	}

	return GetToParentTransformNoBlend(bone, mClipData).GetMatrix4();

}


Transform NEngine::Graphics::Animator::GetToParentTransformNoBlend(const Bone* bone, const ClipData& data) const
{
	auto model = ModelManager::Get()->GetModel(mId);
	const auto& animClip = model->animationClips[data.index];
	auto& animation = animClip.boneAnimations[bone->index];
	if (!animation)
		return Transform();

	return animation->GetTransform(data.currentTick);
}

NMath::Matrix4 NEngine::Graphics::Animator::GetToParentTransformWithBlend(const Bone* bone) const
{
	Transform transforms[BlendOptions::CLIP_COUNT];

	for (int i = 0; i < BlendOptions::CLIP_COUNT; ++i)
	{
		transforms[i] = GetToParentTransformNoBlend(bone, mBlendOptions.clips[i]);
	}

	Transform result; 
	result.position = NMath::Lerp(transforms[0].position, transforms[1].position, mBlendOptions.t);
	result.rotation = NMath::Quaternion::Slerp(transforms[0].rotation, transforms[1].rotation, mBlendOptions.t);


	for (int i = 2; i < BlendOptions::CLIP_COUNT; ++i)
	{
		result.position = NMath::Lerp(result.position, transforms[i].position, mBlendOptions.t);
		result.rotation = NMath::Quaternion::Slerp(result.rotation, transforms[i].rotation, mBlendOptions.t);
	}

	return result.GetMatrix4();
}
