#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = NMath::Normalize({ 1.0f,-1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,    1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,     1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    Mesh groundMesh = MeshBuilder::CreateGroundPlane(20, 20, 1.0f);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture("misc/Concrete.jpg");
    mGround.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mGround.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mGround.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mGround.material.materialPower = 20;

    Mesh ball = MeshBuilder::CreateSphere(60, 60, 0.5f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuseMapId = TextureManager::Get()->LoadTexture("misc/basketball.jpg");
    mBall.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    mBall.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mBall.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    mBall.material.materialPower = 20;

    mBallAnimation = AnimationBuilder()
        .AddPositionKey(Vector3(0.0f, 5.0f, 0.0f), 0.0f)
        .AddPositionKey(Vector3(0.0f, 1.0f, 0.0f), 1.0f, EaseType::EaseInQuad)
        .AddPositionKey(Vector3(0.0f, 5.0f, 0.0f), 2.0f, EaseType::EaseOutQuad)
        .AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 0.0f)
        .AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 0.9f)
        .AddScaleKey(Vector3(1.2f, 0.5f, 1.2f), 1.0f,EaseType::EaseInQuad)
        .AddScaleKey(Vector3(0.8f, 1.3f, 0.8), 1.1f, EaseType::EaseOutQuad)
        .AddScaleKey(Vector3(0.8f, 1.3f, 0.8f), 1.2f)
        .AddScaleKey(Vector3(1.0f, 1.0f, 1.0f), 2.0f, EaseType::EaseOutQuad)
        .AddRotationKey(Quaternion(0.0f, 0.0f, 0.0f, 1.0f), 0.0f)
        .AddRotationKey(Quaternion(0.0f, NMath::Constants::HalfPi, 0.0f, 1.0f), 0.5f)
        .AddRotationKey(Quaternion(0.0f, NMath::Constants::Pi, 0.0f, 1.0f), 1.0f)
        .AddRotationKey(Quaternion(0.0f, NMath::Constants::HalfPi * 3.0f, 0.0f, 1.0f), 1.5f)
        .AddRotationKey(Quaternion(0.0f, NMath::Constants::TwoPi, 0.0f, 1.0f), 2.0f)
        .Build();


        

}

void GameState::Terminate()
{
    mBall.Terminate();
    mStandardEffect.Terminate();
    mGround.Terminate();
}

void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mGround);
    mStandardEffect.End();
}

void GameState::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = (input->IsKeyDown(KeyCode::LSHIFT) ? 25.0f : 5.f) * deltaTime;
    const float turnSpeed = 0.1f * deltaTime;
    if (input->IsKeyDown(KeyCode::W)) {
        mCamera.Walk(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::S)) {
        mCamera.Walk(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::D)) {
        mCamera.Strafe(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::A)) {
        mCamera.Strafe(-moveSpeed);
    }

    if (input->IsKeyDown(KeyCode::E)) {
        mCamera.Rise(moveSpeed);
    }
    else if (input->IsKeyDown(KeyCode::Q)) {
        mCamera.Rise(-moveSpeed);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON)) {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed);
    }

    mAnimationTime += deltaTime;
    const float duration = mBallAnimation.GetDurantion();
    while (mAnimationTime > duration)
    {
        mAnimationTime -= duration;
    }
    mBall.transform = mBallAnimation.GetTransform(mAnimationTime);
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (ImGui::CollapsingHeader("Lighting##", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::DragFloat3("Directional##Light", &mDirectionalLight.direction.x, 0.01f, -0.01f, 1.0f)){
                mDirectionalLight.direction = NMath::Normalize(mDirectionalLight.direction);
            }
            ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
            ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
            ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);
        }
        bool update = false;
       
        mStandardEffect.DebugUI();
    ImGui::End();

    if (!update) return;
}


