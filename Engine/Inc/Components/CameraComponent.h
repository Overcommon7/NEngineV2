#pragma once
#include "Component.h"

namespace NEngine
{
    class CameraComponent : public Component   
    {
        SET_TYPE_ID(ComponentId::Camera);
    public:
        void Initialize() override;
        void Terminate() override;

        Graphics::Camera& GetCamera() { return mCamera; }
        const Graphics::Camera& GetCamera() const { return mCamera; }

    private:
        Graphics::Camera mCamera;
    };
}


