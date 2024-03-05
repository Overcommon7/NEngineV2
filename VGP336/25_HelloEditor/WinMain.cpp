#include <../Engine/Inc/NEngine.h>
#include "GameState.h"
#include "EditorState.h"
#include "EditTemplateState.h"
#include "StateNames.h"

using namespace NEngine;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	AppConfig config;
	config.appName = L"Hello Game World";

	App& myApp = MainApp();
	myApp.AddState<GameState>(State::GAME);
	myApp.AddState<EditorState>(State::EDITOR);
	myApp.AddState<EditTemplateState>(State::TEMPLATE);
	myApp.Run(config);

	return 0;
}