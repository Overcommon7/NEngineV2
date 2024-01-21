#include <../Engine/Inc/NEngine.h>
#include "GameState.h"

using namespace NEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.appName = L"Hello Skeleton Animation";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return 0;
}