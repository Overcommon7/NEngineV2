#include "Precompiled.h"
#include "WNTRengine.h"


NEngine::App& NEngine::MainApp() {
	static App sApp;
	return sApp;
}
