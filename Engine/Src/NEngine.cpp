#include "Precompiled.h"
#include "NEngine.h"


NEngine::App& NEngine::MainApp() {
	static App sApp;
	return sApp;
}
