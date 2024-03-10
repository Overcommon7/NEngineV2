#pragma once
class FirstPersonCameraUpdater
{
public:
    struct Values
    {

    };

    static void Update(Camera* camera, Values& values);
    static void DebugUI(Camera* camera, Values& values);
private:
public:
    FirstPersonCameraUpdater() = delete;
    FirstPersonCameraUpdater(const FirstPersonCameraUpdater& f) = delete;
    FirstPersonCameraUpdater(const FirstPersonCameraUpdater&& f) = delete;
    FirstPersonCameraUpdater& operator=(const FirstPersonCameraUpdater& f) = delete;
    FirstPersonCameraUpdater& operator=(const FirstPersonCameraUpdater&& f) = delete;

};

