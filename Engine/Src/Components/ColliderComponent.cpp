#include "Precompiled.h"
#include "Components/ColliderComponent.h"

#include "SaveUtil.h"

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
	void ColliderComponent::Serialize(rapidjson::Document& doc, rapidjson::Value& value)
	{
		rapidjson::Value componentValue(rapidjson::kObjectType);
		rapidjson::Value shapeValue(rapidjson::kObjectType);
		SaveUtil::SaveString("Type", mLoadingData.shapeType.c_str(), doc, shapeValue);
		if (mLoadingData.shapeType == "Box")
		{
			SaveUtil::SaveVector3("HalfExtents", mLoadingData.param0, doc, shapeValue);
		}
		else if (mLoadingData.shapeType == "Sphere")
		{
			SaveUtil::SaveFloat("Raidus", mLoadingData.param0.x, doc, shapeValue);
		}
		else if (mLoadingData.shapeType == "Hull")
		{
			SaveUtil::SaveVector3("HalfExtents", mLoadingData.param0, doc, shapeValue);
			SaveUtil::SaveVector3("Origin", mLoadingData.param1, doc, shapeValue);
		}
		componentValue.AddMember("Shape", shapeValue, doc.GetAllocator());
		value.AddMember("Collider", componentValue, doc.GetAllocator());
	}
	void ColliderComponent::Deserialize(rapidjson::Value& value)
	{
		if (value.HasMember("Shape"))
		{
			const auto& shapeData = value["Shape"].GetObj();
			if (shapeData.HasMember("Type"))
			{
				mLoadingData.shapeType = shapeData["Type"].GetString();
				if (mLoadingData.shapeType == "Empty")
				{
					mCollisionShape.InitializeEmpty();
				}
				else if (mLoadingData.shapeType == "Box")
				{
					const auto& he = shapeData["HalfExtents"].GetArray();
					mLoadingData.param0 = GetVector3(he);
					mCollisionShape.InitializeBox(mLoadingData.param0);
				}
				else if (mLoadingData.shapeType == "Sphere")
				{
					mLoadingData.param0.x = shapeData["Raidus"].GetFloat();
					mCollisionShape.InitializeSphere(mLoadingData.param0.x);
				}
				else if (mLoadingData.shapeType == "Hull")
				{
					const auto& he = shapeData["HalfExtents"].GetArray();
					const auto& o = shapeData["Origin"].GetArray();
					mLoadingData.param0 = GetVector3(he);
					mLoadingData.param1 = GetVector3(o);
					mCollisionShape.InitializeHull(mLoadingData.param0, mLoadingData.param1);
				}
			}
		}
	}
}