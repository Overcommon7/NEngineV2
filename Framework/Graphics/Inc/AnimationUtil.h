#pragma once
#include "ModelManager.h"

namespace NEngine::Graphics
{
	class Animator;
}

namespace NEngine::Graphics::AnimationUtil
{
	using BoneTransforms = vector<NMath::Matrix4>;

	void ComputeBoneTransform(ModelId modelId, BoneTransforms& boneTransforms, const NEngine::Graphics::Animator* animator = nullptr);
	void ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms);
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
};

