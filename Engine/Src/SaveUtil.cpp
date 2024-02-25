#include "Precompiled.h"
#include "SaveUtil.h"

#define GetStringRef(key) rapidjson::GenericStringRef<char>(key)

void NEngine::SaveUtil::SaveVector3(const char* key, const NMath::Vector3& vector, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	rapidjson::Value vecArray(rapidjson::kArrayType);

	vecArray.PushBack(vector.x, doc.GetAllocator());
	vecArray.PushBack(vector.y, doc.GetAllocator());
	vecArray.PushBack(vector.z, doc.GetAllocator());
	jValue.AddMember(GetStringRef(key), vecArray, doc.GetAllocator());
}

void NEngine::SaveUtil::SaveQuaternion(const char* key, const NMath::Quaternion& quaternion, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	rapidjson::Value qArray(rapidjson::kArrayType);

	qArray.PushBack(quaternion.x, doc.GetAllocator());
	qArray.PushBack(quaternion.y, doc.GetAllocator());
	qArray.PushBack(quaternion.z, doc.GetAllocator());
	qArray.PushBack(quaternion.w, doc.GetAllocator());
	jValue.AddMember(GetStringRef(key), qArray, doc.GetAllocator());
}

void NEngine::SaveUtil::SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	jValue.AddMember(GetStringRef(key), value, doc.GetAllocator());
}

void NEngine::SaveUtil::SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	jValue.AddMember(GetStringRef(key), value, doc.GetAllocator());
}

void NEngine::SaveUtil::SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	jValue.AddMember(GetStringRef(key), value, doc.GetAllocator());
}

void NEngine::SaveUtil::SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	jValue.AddMember(GetStringRef(key), GetStringRef(value), doc.GetAllocator());
}

void NEngine::SaveUtil::SaveStringArray(const char* key, const vector<string>& value, rapidjson::Document& doc, rapidjson::Value& jValue)
{
	rapidjson::Value strArray(rapidjson::kArrayType);

	for (const auto& str : value)
		strArray.PushBack(GetStringRef(str.c_str()), doc.GetAllocator());

	jValue.AddMember(GetStringRef(key), strArray, doc.GetAllocator());	
}

