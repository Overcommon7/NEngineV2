#pragma once
#include "Service.h"

namespace NEngine
{   
    class CameraComponent;

    class CameraService final : public Service    
    {
    public:

        SET_TYPE_ID(ServiceId::Camera);
        const Graphics::Camera& GetMain() const;
        Graphics::Camera& GetMain();

        void SetMainCamera(uint32_t index);

        void Register(CameraComponent* cameraComponent);
        void Unregister(CameraComponent* cameraComponent);

        
    private:
        using CameraEntries = vector<CameraComponent*>;
        CameraEntries mCameraEntries;

        CameraComponent* mMainCamera;



        
    };
}


