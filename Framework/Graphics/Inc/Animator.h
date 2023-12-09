#pragma once

#include "ModelManager.h"
#include "Bone.h"

namespace NEngine::Graphics
{
	class Animator
	{
	public:
		struct ClipData
		{
			int index;
			float currentTick = 0.f;
			float weight;
			bool isLooping = false;
			int priority = 0;


			void StartAnimation(bool isLooping = true)
			{
				currentTick = 0.f;
				this->isLooping = isLooping;
			}
		};

		struct BlendOptions
		{
			std::vector<ClipData> clips;

			inline void SetClipIndex(int clipIndex, int index)
			{
				clips[index].index = clipIndex;
			}

			inline void SetClipLooping(bool looping, int index)
			{
				clips[index].isLooping = looping;
			}

			inline void SetWeight(float weight, int index)
			{
				clips[index].weight = std::clamp(weight, 0.f, 1.f);
			}

			inline void AddClip(int clipIndex, bool looping, int priority, float weight)
			{
				ClipData newClip;
				newClip.index = clipIndex;
				newClip.priority = priority;
				newClip.weight = weight;
				newClip.StartAnimation(looping);
				clips.push_back(newClip);

				std::sort(clips.begin(), clips.end(), [](const ClipData& a, const ClipData& b) {
					return a.priority < b.priority;
					});
			}
		};

		void Initialize(ModelId id);
		void PlayAnimation(int clipIndex, bool looping);
		void Update(float deltaTime);
		void SetBlending(bool blend);

		bool IsFinished(int clipIndex = -1) const;
		bool IsBlending() const;
		size_t GetAnimationCount() const;
		NMath::Matrix4 GetToParentTransform(const Bone* bone) const;

		inline BlendOptions& GetBlendOptions() { return mBlendOptions; }

		void DebugUI();
	private:
		ModelId mId = 0;
		ClipData mClipData;
		bool mIsBlending = false;
		BlendOptions mBlendOptions;
		ClipData newClipData;

		void UpdateNoBlend(float deltaTime, ClipData& clipData, const Model* model);
		void UpdateWithBlend(float deltaTime, const Model* model);

		Transform GetToParentTransformNoBlend(const Bone* bone, const ClipData& data) const;
		NMath::Matrix4 GetToParentTransformWithBlend(const Bone* bone) const;
	};
}
