#pragma once
class FirstPersonMovement
{
public:
    struct Values
    {
        struct Controls
        {
            enum class BindType : size_t
            {
                Left = 0,
                Right = 1,
                Foward = 2,
                Backward = 3,
                Jump = 4,
                Count

            };

            struct Keybind
            {
                KeyCode key;
                bool isDown = false;
                bool pressedThisFrame = false;
                bool releasedThisFrame = false;
            };

            std::array<Keybind, (size_t)BindType::Count> mKeyBinds;

            Controls()
            {
                mKeyBinds[(size_t)BindType::Left].key = KeyCode::A;
                mKeyBinds[(size_t)BindType::Right].key = KeyCode::D;
                mKeyBinds[(size_t)BindType::Foward].key = KeyCode::W;
                mKeyBinds[(size_t)BindType::Backward].key = KeyCode::S;
                mKeyBinds[(size_t)BindType::Jump].key = KeyCode::SPACE;
            }

            Keybind& operator[](BindType bind)
            {
                return mKeyBinds[(size_t)bind];
            }

            const Keybind& operator[](BindType bind) const
            {
                return mKeyBinds[(size_t)bind];
            }
        };


        float mSpeed = 40.f;
        float mJumpForce = 75.f;
        Controls mControls{};

    };

    static void Update(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime);
    static void DebugUI(Rigidbody* rigidbody, Values& values); 
private:
    using Keybind = Values::Controls::Keybind;
    using BindType = Values::Controls::BindType;
    static void UpdateInput(Values::Controls& contols);
    static Vector3 UpdateMovement(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime);
    static Vector3 UpdateJump(Rigidbody* rigidbody, Values& values, Camera* camera, float deltaTime);
public:
    FirstPersonMovement() = delete;
    FirstPersonMovement(const FirstPersonMovement& f) = delete;
    FirstPersonMovement(const FirstPersonMovement&& f) = delete;
    FirstPersonMovement& operator=(const FirstPersonMovement& f) = delete;
    FirstPersonMovement& operator=(const FirstPersonMovement&& f) = delete;

};

