#include <StzEngine/inc/StzEngine.h>
#include "ShapeStates.h"

using namespace StzEngine;
using namespace StzEngine::Core;
using namespace StzEngine::Input;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	StzEngine::AppConfig config;
	config.appName = L"Hello Shapes";

	StzEngine::App& myApp = StzEngine::MainApp();

	myApp.AddState<ShapeState>("ShapeState");
	myApp.AddState<QuadState>("QuadState");
	myApp.AddState<TriForceState>("TriForceState");
	myApp.AddState<HeartState>("HeartState");
	myApp.Run(config);
	
	return 0;
}