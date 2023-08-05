#include <../Engine/Inc/WNTRengine.h>
#include "GameState.h"

using namespace NEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	AppConfig config;
	config.appName = L"Hello Mesh Builder";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.Run(config);

	return (0);
}