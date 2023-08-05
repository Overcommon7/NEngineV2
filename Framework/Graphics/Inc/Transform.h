#pragma once
#include  "Common.h"

namespace NEngine::Graphics 
{
	struct Transform 
	{
		NMath::Vector3 position = NMath::Vector3::Zero;
		NMath::Quaternion rotation = NMath::Quaternion::Identity;
		NMath::Vector3 scale = NMath::Vector3::One;

		NMath::Matrix4 GetMatrix4() const
		{
			return{
				NMath::Matrix4::Scaling(scale) *
				NMath::Matrix4::MatrixRotationQuaternion(rotation) *
				NMath::Matrix4::Translation(position)
			};
		}
	};

}