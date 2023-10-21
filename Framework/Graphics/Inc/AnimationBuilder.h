#pragma once

#include "Animation.h"

namespace NEngine::Graphics
{
	class AnimationBuilder final
	{
	public:
		AnimationBuilder& AddPositionKey(const NMath::Vector3& position, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddScaleKey(const NMath::Vector3& sacle, float time, EaseType easeType = EaseType::Linear);
		AnimationBuilder& AddRotationKey(const NMath::Quaternion& rotation, float time, EaseType easeType = EaseType::Linear);

		[[nodiscard]] Animation Build();
	private:

		
		Animation workingCopy;
	};
}
