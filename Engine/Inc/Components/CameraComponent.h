#pragma once
#include "Component.h"

namespace NEngine
{
    class CameraComponent : public Component   
    {       
    public:
        SET_TYPE_ID(ComponentId::Camera);
        void Initialize() override;
        void Terminate() override;

        void Serialize(rapidjson::Document& doc, rapidjson::Value& value) override;
        void Deserialize(rapidjson::Value& value) override;

        Graphics::Camera& GetCamera() { return mCamera; }
        const Graphics::Camera& GetCamera() const { return mCamera; }

    protected:
        Graphics::Camera mCamera;
        NMath::Vector3 mStartingPosition = NMath::Vector3::Zero;
        NMath::Vector3 mStartingLookAt = NMath::Vector3::Zero;
    };
}


