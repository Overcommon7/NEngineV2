#pragma once

#include "Keyframe.h"
#include "Transform.h"

namespace NEngine::Graphics
{
	class Animation
	{
	public:
		Transform GetTransform(float time) const;
		inline float GetDurantion() const { return mDuration; };

	private:
		friend class AnimationBuilder;

		NMath::Vector3 GetPosition(float time) const;
		NMath::Quaternion GetRotation(float time) const;
		NMath::Vector3 GetScale(float time) const;


		PositionKeys mPositionKeys;
		RotationKeys mRotationKeys;
		ScaleKeys mScaleKeys;
		float mDuration;
	};
}
