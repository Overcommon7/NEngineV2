#pragma once
class ImUtils
{
public:
    static bool SerializeInt(const string& name, int& value);
    static bool SerializeVector3(const string& name, Vector3& vector, float speed = 0.5f);
    static bool SerializeFloat(const string& name, float& num, float speed = 0.5f);
    static bool SerializeBool(const string& name, bool& value);
    
    static void DrawString(const string& name, const string& str);
    static void DrawBool(const string& name, bool state);
    static void DrawInt(const string& name, int value);
    static void DrawFloat(const string& name, float value);
    static void DrawVector3(const string& name, const Vector3& vector);
private:
public:
    ImUtils() = delete;
    ImUtils(const ImUtils& i) = delete;
    ImUtils(const ImUtils&& i) = delete;
    ImUtils& operator=(const ImUtils& i) = delete;
    ImUtils& operator=(const ImUtils&& i) = delete;

};

