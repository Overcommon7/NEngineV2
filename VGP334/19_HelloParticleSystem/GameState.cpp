#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 2.0f,-8.0f });
    mCamera.SetLookAt({ 0.0f, 1.50f, 0.0f });

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

    mGroundShape.InitializeHull({ 10.f, 0.5f, 10.0f }, { 0.f, -0.5f, 0.f });
    mGroundRigidbody.Initialize(mGround.transform, mGroundShape);



    mParticleEffect.Initialize();
    mParticleEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.lifeTime = std::numeric_limits<float>::max();
    info.maxParticles = 100;
    info.spawnPosition = { 0, 1, 0 };
    info.spawnDirection = { 0, 1, 0 };
    info.spawnDelay = 0;
    info.minParticlesPerEmit = 1;
    info.maxParticlesPerEmit = 3;
    info.minTimeBetweenParticles = 0.1f;
    info.maxTimeBetweenParticles = 0.3f;
    info.minSpawnAngle = NMath::Constants::HalfPi * 0.25f;
    info.maxSpawnAngle = -NMath::Constants::HalfPi * 0.25f;
    info.maxParticles = 100;
    info.minSpeed = 2.0f;
    info.maxSpeed = 7.0f;

    info.particleInfo.lifetime = 2.0f;

    info.particleInfo.colorStart = Colors::OrangeRed;
    (info.particleInfo.colorEnd = Colors::LightYellow).a = 0.1f;

    info.particleInfo.scaleStart = Vector3::One;
    info.particleInfo.scaleEnd = { 0.1f, 0.1f, 0.1f };

    mParticleSystem.Initialize(info);

}

void GameState::Terminate()
{
    mGroundRigidbody.Terminate();

    mGroundShape.Terminate();

    mStandardEffect.Terminate();
    mGround.Terminate();
    mParticleEffect.Terminate();
}

void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mGround);
    mStandardEffect.End();

    mParticleEffect.Begin();
        mParticleSystem.Render(mParticleEffect);
    mParticleEffect.End();
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

    mParticleSystem.Update(deltaTime);
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
        mParticleSystem.DebugUI();
        mStandardEffect.DebugUI();
        PhysicsWorld::Get()->DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCamera);
}


