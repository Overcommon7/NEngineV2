#include "pch.h"
#include "GameState.h"

using namespace NEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.appName = L"Hello Game World";
	config.winHeight = 1080;
	config.winWidth = 1920;

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);
	

	return 0;
}