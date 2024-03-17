#pragma once
class FirstPersonAnimation
{
public:
    static void Update(Animator* animator, Vector3 velocity);
private:
    static void SetDirection(Animator::BlendOptions& options, int direction);
public:
    FirstPersonAnimation() = delete;
    FirstPersonAnimation(const FirstPersonAnimation& f) = delete;
    FirstPersonAnimation(const FirstPersonAnimation&& f) = delete;
    FirstPersonAnimation& operator=(const FirstPersonAnimation& f) = delete;
    FirstPersonAnimation& operator=(const FirstPersonAnimation&& f) = delete;

};

