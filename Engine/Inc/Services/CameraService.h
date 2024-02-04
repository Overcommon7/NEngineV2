#pragma once
#include "Service.h"

namespace NEngine
{   
    class CameraService final : public Service    
    {
    public:

        SET_TYPE_ID(ServiceId::Camera);
        const Graphics::Camera& GetMain() const;
        Graphics::Camera& GetMain();

        void Deserialize(rapidjson::Value& value) override;
        void SetMainCamera(uint32_t index);

        void DebugUI() override;

        
    private:
        friend class CameraComponent;

        using CameraEntries = vector<CameraComponent*>;
        CameraEntries mCameraEntries;

        CameraComponent* mMainCamera;

        void Register(CameraComponent* cameraComponent);
        void Unregister(CameraComponent* cameraComponent);

        
    };
}


