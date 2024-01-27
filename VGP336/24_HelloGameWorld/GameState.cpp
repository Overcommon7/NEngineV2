#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    {
        auto cameraComponent = mCamera_1.AddComponent<CameraComponent>();
        auto& mCamera = cameraComponent->GetCamera();
        mCamera.SetPosition({ 0.0f, 2.0f,-8.0f });
        mCamera.SetLookAt({ 0.0f, 1.50f, 0.0f });
        cameraService.Register(cameraComponent);
        cameraService.SetMainCamera(0);
    }

    {
        auto cameraComponent = mCamera_2.AddComponent<CameraComponent>();
        auto& mCamera = cameraComponent->GetCamera();
        mCamera.SetPosition({ 0.0f, 2.0f,-8.0f });
        mCamera.SetLookAt({ 0.0f, 1.50f, 0.0f });
        cameraService.Register(cameraComponent);
    }
   

    mDirectionalLight.direction = NMath::Normalize({ 1.0f,-1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,    1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f, 1.0f };

    GameObjectFactory::Make("../../Assets/Templates/test_object.json", mGameObject);
    mGameObject.Initialize();
}

void GameState::Terminate()
{;
    mGameObject.Terminate();
}

void GameState::Render()
{

}

void GameState::Update(float deltaTime)
{
    auto& mCamera = cameraService.GetMain();
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

    if (input->IsKeyPressed(KeyCode::ONE))
        cameraService.SetMainCamera(1);
    if (input->IsKeyPressed(KeyCode::ZERO))
        cameraService.SetMainCamera(0);

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
 
        mGameObject.DebugUI();
    ImGui::End();

    SimpleDraw::Render(cameraService.GetMain());
}


