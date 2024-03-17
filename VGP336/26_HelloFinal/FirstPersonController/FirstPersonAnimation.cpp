#include "pch.h"
#include "FirstPersonAnimation.h"

void FirstPersonAnimation::Update(Animator* animator, Vector3 velocity)
{
	static constexpr int LEFT = 0;
	static constexpr int RIGHT = 1;
	static constexpr int FOWARD = 2;
	static constexpr int BACKWARD = 3;
	static constexpr int NONE = 5;
	auto& options = animator->GetBlendOptions();

	if (velocity.x == 0 && velocity.z == 0)
	{
		SetDirection(options, FOWARD);
	}
	else if (velocity.x != 0 && velocity.z != 0)
	{
		const bool XLessthanZ = velocity.x < velocity.z;
		const float fraction = XLessthanZ ? velocity.x / velocity.z : velocity.z < velocity.x;
		const float weightX = XLessthanZ ? fraction : 1 - fraction;
		const float weightZ = XLessthanZ ? 1 - fraction : fraction;

		SetDirection(options, NONE);
		options.SetWeight(weightX, velocity.x > 0 ? RIGHT : LEFT);
		options.SetWeight(weightZ, velocity.z > 0 ? FOWARD : BACKWARD);

	}
	else if (velocity.x != 0)
	{
		SetDirection(options, velocity.x > 0 ? RIGHT : LEFT);
	}
	else if (velocity.z != 0)
	{
		SetDirection(options, velocity.z > 0 ? FOWARD : BACKWARD);
	}
}

void FirstPersonAnimation::SetDirection(Animator::BlendOptions& options, int direction)
{
	for (int i = 0; i < 4; ++i)
	{
		if (i == direction)
			options.SetWeight(1, i);
		else
			options.SetWeight(0, i);
	}
}
