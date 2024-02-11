#include "Precompiled.h"
#include "Components/ColliderComponent.h"

namespace
{
	NEngine::NMath::Vector3 GetVector3(const rapidjson::GenericArray<false, rapidjson::Value>& arr)
	{
		float x = arr[0].GetFloat();
		float y = arr[1].GetFloat();
		float z = arr[2].GetFloat();
		return Vector3(x, y, z);
	}
}

namespace NEngine
{
	void ColliderComponent::Initialize()
	{
	    
	}
	void ColliderComponent::Terminate()
	{
		mCollisionShape.Terminate();
	}
	void ColliderComponent::Deserialize(rapidjson::Value& value)
	{
		if (value.HasMember("Shape"))
		{
			const auto& shapeData = value["Shape"].GetObj();
			if (shapeData.HasMember("Type"))
			{
				const string shapeType(shapeData["Type"].GetString());
				if (shapeType == "Empty")
				{
					mCollisionShape.InitializeEmpty();
				}
				else if (shapeType == "Box")
				{
					const auto& he = shapeData["HalfExtents"].GetArray();
					mCollisionShape.InitializeBox(GetVector3(he));
				}
				else if (shapeType == "Sphere")
				{
					float radius = shapeData["Raidus"].GetFloat();
					mCollisionShape.InitializeSphere(radius);
				}
				else if (shapeType == "Hull")
				{
					const auto& he = shapeData["HalfExtents"].GetArray();
					const auto& o = shapeData["Origin"].GetArray();
					mCollisionShape.InitializeHull(GetVector3(he), GetVector3(o));
				}
			}
		}
	}
}