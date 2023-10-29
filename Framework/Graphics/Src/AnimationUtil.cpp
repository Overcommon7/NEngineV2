#include "Precompiled.h"
#include "AnimationUtil.h"

#include "SimpleDraw.h"
#include "Colors.h"
#include "Animator.h"

using namespace NEngine::Graphics::AnimationUtil;
using namespace NEngine::Graphics;
using namespace NEngine;

namespace
{
	void ComputeBoneTransformRecursize(const Bone const* bone, BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (!bone) return;
		
		if (animator)
		{
			boneTransforms[bone->index] = animator->GetToParentTransform(bone);
		}
		else
		{
			boneTransforms[bone->index] = bone->toParentTransform;
		}
		
		if (bone->parent)
		{
			boneTransforms[bone->index] =
				boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
		}

		for (auto child : bone->children)
		{
			ComputeBoneTransformRecursize(child, boneTransforms, animator);
		}
	}
}

void NEngine::Graphics::AnimationUtil::
	ComputeBoneTransform(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (model->skeleton != nullptr)
	{
		boneTransforms.resize(model->skeleton->bones.size(), NMath::Matrix4::Identity);
		ComputeBoneTransformRecursize(model->skeleton->root, boneTransforms, animator);

		for (auto& bone : model->skeleton->bones)
		{
			if (!bone->parent) continue;
			Bone* boneChain = bone.get();
			while (boneChain->parent)
			{
				boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
				boneChain = boneChain->parent;
			}
				
		}
	}
}

void NEngine::Graphics::AnimationUtil::
	ApplyBoneOffsets(ModelId modelId, BoneTransforms& boneTransforms)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (model->skeleton)
	{
		for (auto& bone : model->skeleton->bones)
		{
			boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
		}
	}
}

void NEngine::Graphics::AnimationUtil::
	DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
	auto model = ModelManager::Get()->GetModel(modelId);
	if (!model->skeleton) return;

	for (auto& bone : model->skeleton->bones)
	{
		if (bone->parent)
		{
			const auto posA = NMath::GetTranslation(boneTransforms[bone->index]);
			const auto posB = NMath::GetTranslation(boneTransforms[bone->parentIndex]);
			SimpleDraw::AddLine(posA, posB, Colors::HotPink);
			SimpleDraw::AddSphere(10, 10, 0.025f, posA, Colors::Green);
		}
	}
}
