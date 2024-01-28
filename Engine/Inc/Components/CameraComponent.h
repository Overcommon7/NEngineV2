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

        void Deserialize(rapidjson::Value& value) override;

        Graphics::Camera& GetCamera() { return mCamera; }
        const Graphics::Camera& GetCamera() const { return mCamera; }

    protected:
        Graphics::Camera mCamera;
    };
}


