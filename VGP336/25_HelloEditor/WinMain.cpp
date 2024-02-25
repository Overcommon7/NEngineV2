#include <../Engine/Inc/NEngine.h>
#include "GameState.h"
#include "EditorState.h"
#include "EditTemplateState.h"

using namespace NEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.appName = L"Hello Game World";

	App& myApp = MainApp();
	myApp.AddState<GameState>("GameState");
	myApp.AddState<EditorState>("EditState");
	myApp.AddState<EditTemplateState>("EditTemplateState");
	myApp.Run(config);

	return 0;
}