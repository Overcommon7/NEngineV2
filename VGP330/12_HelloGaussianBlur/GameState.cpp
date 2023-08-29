#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f,-15.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = NMath::Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.diffuse = { 0.8f, 0.8f, 0.8f, 1.0f };
    mDirectionalLight.specular = { 1.0f, 1.0f, 1.0f, 1.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mPostProceessingEffect.Initialize(L"../../Assets/Shaders/PostProcess.fx");
    mPostProceessingEffect.SetTexture(&mBaseRenderTarget);

    auto gs = GraphicsSystem::Get();
    const auto screenWidth = gs->GetBackBufferWidth();
    const auto screenHeight = gs->GetBackBufferHeight();

    mBaseRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
    mBloomRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    MeshPX screenMesh = MeshBuilder::CreateScreenQuad();
    mScreenQuad.meshBuffer.Initialize(screenMesh);

    auto tm = TextureManager::Get();
    tm->SetRootDirectory("H:/LaSalle/Courses/Quarter8/NEngine/Assets/Textures/");

    mSun.diffuseMapId = tm->LoadTexture(L"planets/sun.jpg");
    mSun.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 512, 5.0f));
    mSun.material.emissive = Colors::White;
    mSun.material.materialPower = 1.0f;

    mEarth.diffuseMapId = tm->LoadTexture(L"planets/earth/earth.jpg");
    mEarth.normalMapId = tm->LoadTexture(L"planets/earth/earth_normal.jpg");
    mEarth.specMapId = tm->LoadTexture(L"planets/earth/earth_spec.jpg");
    mEarth.bumpMapId = tm->LoadTexture(L"planets/earth/earth_bump.jpg");


    mEarth.meshBuffer.Initialize(MeshBuilder::CreateSphere(1024, 512, 1.0f));
    mEarth.material.ambient = { 0.5f, 0.5f, 0.5f, 1.0f };
    mEarth.material.diffuse = { 0.8f, 0.5f, 0.5f, 1.0f };
    mEarth.material.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
    mEarth.material.materialPower = 10.0f;
    mEarth.transform.position = { 0.0f, 0.f, -8.0f };
}

void GameState::Terminate()
{
    mEarth.Terminate();
    mSun.Terminate();
    mScreenQuad.Terminate();
    mBloomRenderTarget.Terminate();
    mBaseRenderTarget.Terminate();
    mPostProceessingEffect.Terminate();
    mStandardEffect.Terminate();
}

void GameState::Render()
{
    mBaseRenderTarget.BeginRender();
    mStandardEffect.Begin();
    mStandardEffect.Render(mSun);
    mStandardEffect.Render(mEarth);
    mStandardEffect.End();

    SimpleDraw::AddGroundPlane(20, Colors::White);
    SimpleDraw::Render(mCamera);

    mBaseRenderTarget.EndRender();

    mPostProceessingEffect.Begin();
    mPostProceessingEffect.Render(mScreenQuad);
    mPostProceessingEffect.End();
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
        mPostProceessingEffect.DebugUI();
    ImGui::End();

}


