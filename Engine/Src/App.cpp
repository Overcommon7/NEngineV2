#include "Precompiled.h"
#include "App.h"
#include "AppState.h"
#include "Graphics/Inc/DebugUI.h"


using namespace NEngine;
using namespace NEngine::Core;
using namespace NEngine::Input;
using namespace NEngine::Graphics;
using namespace NEngine::Physics;

void App::ChangeState(const std::string& stateName)
{
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}

void App::Run(const AppConfig& config)
{
	// create window
	Window myWindow;
	myWindow.Initialize(
		GetModuleHandle(nullptr),
		config.appName,
		config.winWidth,
		config.winHeight
	);

	ASSERT(myWindow.IsActive(), "Failed to create window!");

	// initialize static classes
	auto handle = myWindow.GetWindowHandle();

	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle,false);
	DebugUI::StaticInitialize(handle, false, false);
	SimpleDraw::StaticInitialize(config.debugDrawLimit);
	TextureManager::StaticInitialize("../../Assets/Textures/");
	ModelManager::StaticInitialize();

	PhysicsWorld::Settings settings;
	PhysicsWorld::StaticInitialize(settings);

	auto physicsWorld = PhysicsWorld::Get();
	auto inputSystem = InputSystem::Get();

	ASSERT(mCurrentState != nullptr, "App -- need an app state");
	mCurrentState->Initialize();
	mRunning = true;
	while (mRunning)
	{
		myWindow.ProcessMessage();
		inputSystem->Update();


		if (!myWindow.IsActive() || inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			break;
		}

		if (mNextState != nullptr)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}
		
		//run the game
		auto deltaTime = TimeUtil::GetDeltaTime();

		if (deltaTime < 0.5f)
		{
			physicsWorld->Update(deltaTime);
			mCurrentState->Update(deltaTime);
		}

		auto graphicSystem = GraphicsSystem::Get();
		graphicSystem->BeginRender();
		mCurrentState->Render();
			DebugUI::BeginRender();
				mCurrentState->DebugUI();
			DebugUI::EndRender();
		graphicSystem->EndRender();
	}

	mCurrentState->Terminate();

	//terminate static classes
	PhysicsWorld::StaticTerminate();
	TextureManager::StaticTerminate();
	ModelManager::StaticTerminate();
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	InputSystem::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	myWindow.Terminate();
}

void App::Quit()
{
	mRunning = false;
}
