#include "GameState.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Input;

void GameState::Initialize()
{
    mWorld.AddService<CameraService>();
    mWorld.AddService<UpdateService>();
    mWorld.Intialize(50);

    mWorld.CreateGameObject("../../Assets/Templates/test_fps_camera.json");
    mWorld.CreateGameObject("../../Assets/Templates/test_object.json");

    

   

    mDirectionalLight.direction = NMath::Normalize({ 1.0f,-1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,    1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f, 1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f, 1.0f };

    
}

void GameState::Terminate()
{
    mWorld.Terminate();
}

void GameState::Render()
{
    mWorld.Render();
}

void GameState::Update(float deltaTime)
{

    mWorld.Update(deltaTime);

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
    ImGui::End();

    mWorld.DebugUI();
}


