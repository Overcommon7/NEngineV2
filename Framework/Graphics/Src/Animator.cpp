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
	mIsBlending = blend && mBlendOptions.clips.size() > 1;
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

void NEngine::Graphics::Animator::DebugUI()
{
	if (ImGui::CollapsingHeader("Animator##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		
		ImGui::Checkbox("Use Blending", &mIsBlending);

		if (!mIsBlending)
		{
			int clip = mClipData.index;
			if (ImGui::InputInt("Clip", &clip))
			{
				if (clip < 0) return;
				if (clip >= GetAnimationCount()) return;

				PlayAnimation(clip, true);
			}
		}
		else
		{
			const string clipLabel = "Clip##";
			const string loopingLabel = "Looping##";
			const string blendLabel = "Blend Weight##";
			const string clipDataLabel = "ClipData: ";

			for (int i = 0; i < mBlendOptions.clips.size(); ++i)
			{
				const string number = to_string(i + 1);
				if (ImGui::CollapsingHeader((clipDataLabel + number).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					int clip = mBlendOptions.clips[i].index;
					if (ImGui::InputInt((clipLabel + number).c_str(), &clip))
					{
						if (clip < 0) continue;
						if (clip >= GetAnimationCount()) continue;
						mBlendOptions.SetClipIndex(clip, i);
					}

					bool isLooping = mBlendOptions.clips[i].isLooping;
					if (ImGui::Checkbox((loopingLabel + number).c_str(), &isLooping))
					{
						mBlendOptions.SetClipLooping(isLooping, i);
					}

					ImGui::DragFloat((blendLabel + number).c_str(), &mBlendOptions.clips[i].weight, 0.01f, 0, 1);
				}
			}		
		}

		if (ImGui::CollapsingHeader("Set Up New Clip##", ImGuiTreeNodeFlags_DefaultOpen))
		{
			int index = newClipData.index;
			if (ImGui::InputInt("New Clip Index", &index))
			{
				if (index >= 0 && index < GetAnimationCount())
					newClipData.index = index;
			}

			ImGui::Checkbox("New Clip Looping", &newClipData.isLooping);
			ImGui::DragFloat("New Clip Blend Weight", &newClipData.weight, 0.01f, 0, 1);
			ImGui::InputInt("New Clip Data Priority", &newClipData.priority);

			if (ImGui::Button("Add Clip"))
			{
				mBlendOptions.AddClip(newClipData.index, newClipData.isLooping, newClipData.priority, newClipData.weight);
			}
		}
		
	}

	
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
	vector<Transform> transforms;
	size_t size = mBlendOptions.clips.size();

	for (int i = 0; i < size; ++i)
	{
		transforms.emplace_back(GetToParentTransformNoBlend(bone, mBlendOptions.clips[i]));
	}

	
	float totalWeight = 0;

	for (auto& clip : mBlendOptions.clips)
	{
		totalWeight += clip.weight;
	}

	if (totalWeight == 0) totalWeight = 1;


	Transform result;
	result.position = transforms[0].position;
	result.rotation = transforms[0].rotation;
	for (int i = 1; i < size; ++i)
	{
		result.position = NMath::Lerp(result.position, transforms[i].position, mBlendOptions.clips[i].weight / totalWeight);
		result.scale = NMath::Lerp(result.scale, transforms[i].scale, mBlendOptions.clips[i].weight / totalWeight);
		result.rotation = NMath::Quaternion::Slerp(result.rotation, transforms[i].rotation, mBlendOptions.clips[i].weight / totalWeight);
	}

	return result.GetMatrix4();
}
