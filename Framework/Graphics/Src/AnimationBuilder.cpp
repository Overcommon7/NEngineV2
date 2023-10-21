#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace NEngine::Graphics;
using namespace NEngine;
using namespace NEngine::NMath;

namespace
{
	template<class T>
	inline void PushKey(Keyframes<T>& keyframes, const T& value, float time, EaseType easeType)
	{
		ASSERT(keyframes.empty() || keyframes.back().time <= time, 
			"AnimationBuilder: cannot add keyframe to back in time");
		keyframes.emplace_back(value, time, easeType);
	}
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const NMath::Vector3& position, float time, EaseType easeType)
{
	PushKey(workingCopy.mPositionKeys, position, time, easeType);
	workingCopy.mDuration = std::max(workingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const NMath::Vector3& scale, float time, EaseType easeType)
{
	PushKey(workingCopy.mScaleKeys, scale, time, easeType);
	workingCopy.mDuration = std::max(workingCopy.mDuration, time);
	return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const NMath::Quaternion& rotation, float time, EaseType easeType)
{
	PushKey(workingCopy.mRotationKeys, rotation, time, easeType);
	workingCopy.mDuration = std::max(workingCopy.mDuration, time);
	return *this;
}

Animation NEngine::Graphics::AnimationBuilder::Build()
{
	ASSERT(!workingCopy.mPositionKeys.empty() || !workingCopy.mRotationKeys.empty() || !workingCopy.mScaleKeys.empty(),
		"AnimationBuilder: no animation keys availible");
	return std::move(workingCopy);
}
