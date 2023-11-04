#include "Precompiled.h"
#include "Animation.h";

using namespace NEngine::Graphics;
using namespace NEngine;
using namespace NEngine::NMath;

namespace
{
	float GetLerpTime(float startTime, float endTime, float time, EaseType easeType)
	{
		
		float t = (time - startTime) / (endTime - startTime);

		switch (easeType)
		{
		case EaseType::EaseInQuad:
			t = t * t;  break;
		case EaseType::EaseOutQuad:
			t = t * (2.0f - t);
			break;
		case EaseType::EaseInOutQuad:
			t *= 2.0f;
			if (t < 1.f)
			{
				t = 0.5f * t * t;
			}
			else
			{
				t -= 1.f;
			}
			break;
		}
		return t;
	}
}

Transform Animation::GetTransform(float time) const
{
	auto transform = Transform();

	transform.position = GetPosition(time);
	transform.rotation = GetRotation(time);
	transform.scale = GetScale(time);

	return transform;
}

Vector3 Animation::GetPosition(float time) const
{
	for (uint32_t i = 0; i < mPositionKeys.size(); i++)
	{
		if (time > mPositionKeys[i].time) continue;

		if (i > 0)
		{
			float t = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time, mPositionKeys[i].easeType);
			return NMath::Lerp(mPositionKeys[i - 1].Key, mPositionKeys[i].Key, t);
		}
		else
		{
			return mPositionKeys.front().Key;
		}
	}

	if (!mPositionKeys.empty())
	{
		return mPositionKeys.back().Key;
	}

	return Vector3::Zero;
}

Quaternion Animation::GetRotation(float time) const
{

	for (uint32_t i = 0; i < mRotationKeys.size(); i++)
	{
		if (time > mRotationKeys[i].time) continue;

		if (i > 0)
		{
			float t = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
			return NMath::Quaternion::Slerp(mRotationKeys[i - 1].Key, mRotationKeys[i].Key, t);
		}
		else
		{
			return mRotationKeys[i].Key;
		}
	}

	if (!mRotationKeys.empty())
	{
		return mRotationKeys.back().Key;
	}

	return Quaternion::Identity;
}

Vector3 Animation::GetScale(float time) const
{

	for (uint32_t i = 0; i < mScaleKeys.size(); i++)
	{
		if (time > mScaleKeys[i].time) continue;

		if (i > 0)
		{
			float t = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time, mScaleKeys[i].easeType);
			return NMath::Lerp(mScaleKeys[i - 1].Key, mScaleKeys[i].Key, t);
		}
		else
		{
			return mScaleKeys.front().Key;
		}
	}

	if (!mScaleKeys.empty())
	{
		return mScaleKeys.back().Key;
	}

	return Vector3::One;
}
