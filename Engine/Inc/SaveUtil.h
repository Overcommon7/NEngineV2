#pragma once
namespace NEngine
{
	class SaveUtil
	{
    public:
        static void SaveVector3(const char* key, const NMath::Vector3& vector, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveQuaternion(const char* key, const NMath::Quaternion& quaternion, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveFloat(const char* key, float value, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveInt(const char* key, int value, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveBool(const char* key, bool value, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveString(const char* key, const char* value, rapidjson::Document& doc, rapidjson::Value& jValue);
        static void SaveStringArray(const char* key, const vector<string>& value, rapidjson::Document& doc, rapidjson::Value& jValue);
    private:
       
    public:
        SaveUtil() = delete;
        SaveUtil(const SaveUtil& s) = delete;
        SaveUtil(const SaveUtil&& s) = delete;
        SaveUtil& operator=(const SaveUtil& s) = delete;
        SaveUtil& operator=(const SaveUtil&& s) = delete;

	};
}


