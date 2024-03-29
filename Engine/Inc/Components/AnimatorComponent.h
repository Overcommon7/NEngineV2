#pragma once
#include "Component.h"
namespace NEngine
{
    class AnimatorComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::Animator);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;
        void DebugUI() override;

        bool Play(int index, bool looping = true);

        Graphics::Animator& GetAnimator() { return mAnimator; }
        const Graphics::Animator& GetAnimator() const { return mAnimator; }
    private:
        Graphics::Animator mAnimator;
    };
}


