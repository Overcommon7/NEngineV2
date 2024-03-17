#pragma once
class FirstPersonCameraUpdater
{
public:
    struct Values
    {
        float sensitivity = 0.25f;
        float fov = 60;
        Vector3 offset = { 0, 2, 0 };
        Vector3 direction{};
    };

    static void Update(Camera* camera, Values& values, NEngine::Transform* cameraTransform, NEngine::Transform* playerTransform, float deltaTime);
    static void DebugUI(Camera* camera, Values& values);
private:
    inline static bool imGuiHovered = false;
public:
    FirstPersonCameraUpdater() = delete;
    FirstPersonCameraUpdater(const FirstPersonCameraUpdater& f) = delete;
    FirstPersonCameraUpdater(const FirstPersonCameraUpdater&& f) = delete;
    FirstPersonCameraUpdater& operator=(const FirstPersonCameraUpdater& f) = delete;
    FirstPersonCameraUpdater& operator=(const FirstPersonCameraUpdater&& f) = delete;

};

