#pragma once

#include "Common.h"

namespace NEngine::Graphics
{
	enum class EaseType
	{
		Linear,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,


	};

	template<class T>
	struct Keyframe
	{
		Keyframe() = default;
		Keyframe(const T& key, float time, EaseType easeType = EaseType::Linear)
			: Key(key), time(time), easeType(easeType)
		{

		}

		T Key;
		float time;
		EaseType easeType = EaseType::Linear;
	};

	template<class T>
	using Keyframes = vector<Keyframe<T>>;

	using PositionKeys = Keyframes<NMath::Vector3>;
	using RotationKeys = Keyframes<NMath::Quaternion>;
	using ScaleKeys = Keyframes<NMath::Vector3>;
}
