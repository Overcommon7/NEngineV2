#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = NMath::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mTerrainEffect.Initialize();
    mTerrainEffect.SetCamera(mCamera);
    mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mTerrainEffect.SetDirectionalLight(mDirectionalLight);

    auto model = ModelManager::Get()->LoadModel("../../Assets/Models/space-soldier/space-soldier.model");
    mCharacter = CreateRenderGroup(model);

    mTerrain.Initialize("../../Assets/Textures/terrain/heightmap_512x512.raw", 15.f);

    ground.meshBuffer.Initialize(mTerrain.mesh);
    ground.diffuseMapId = TextureManager::Get()->LoadTexture("terrain/grass_2048.jpg");
    ground.specMapId = TextureManager::Get()->LoadTexture("terrain/dirt_seamless.jpg");
    ground.material.ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
    ground.material.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    ground.material.specular = { 0.9f, 0.9f, 0.9f, 1.0f };
    ground.material.materialPower = 20;

}

void GameState::Terminate()
{
    CleanupRenderGroup(mCharacter);
    mStandardEffect.Terminate();
    ground.Terminate();
    mShadowEffect.Terminate();
    mTerrainEffect.Terminate();
}

void GameState::Render()
{
    mShadowEffect.Begin();
        DrawRenderGroup(mShadowEffect, mCharacter);
    mShadowEffect.End();

    mTerrainEffect.Begin();
        mTerrainEffect.Render(ground);
    mTerrainEffect.End();

    mStandardEffect.Begin();
        DrawRenderGroup(mStandardEffect, mCharacter);
        //mStandardEffect.Render(ground);
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

    mShadowEffect.SetFocus({ mCamera.GetPosition().x, 0, mCamera.GetPosition().z });

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
        mStandardEffect.DebugUI();
        mShadowEffect.DebugUI();
        mTerrainEffect.DebugUI();
    ImGui::End();

}


