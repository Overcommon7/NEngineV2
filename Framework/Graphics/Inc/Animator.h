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
			bool isLooping = false;

			void StartAnimation(bool isLooping)
			{
				currentTick = 0.f;
				isLooping = true;
			}
		};

		struct BlendOptions
		{
			static const size_t CLIP_COUNT = 2;

			std::array<ClipData, CLIP_COUNT> clips;
			float t;
			

			inline void SetClipIndex(int clipIndex, int index)
			{
				clips[index].index = clipIndex;
			}

			inline void SetClipLooping(bool looping, int index)
			{
				clips[index].isLooping = looping;
			}

			inline void SetUpClip(int clipIndex, bool looping, int index)
			{
				clips[index].index = clipIndex;
				clips[index].isLooping = looping;
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
	private:
		ModelId mId = 0;
		ClipData mClipData;
		bool mIsBlending = false;
		BlendOptions mBlendOptions;

		void UpdateNoBlend(float deltaTime, ClipData& clipData, const Model* model);
		void UpdateWithBlend(float deltaTime, const Model* model);

		Transform GetToParentTransformNoBlend(const Bone* bone, const ClipData& data) const;
		NMath::Matrix4 GetToParentTransformWithBlend(const Bone* bone) const;
	};
}
