#include <StzEngine/inc/StzEngine.h>
#include "ShapeStates.h"

using namespace StzEngine;
using namespace StzEngine::Core;
using namespace StzEngine::Input;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
	StzEngine::AppConfig config;
	config.appName = L"Hello Textured Mesh Builder";

	StzEngine::App& myApp = StzEngine::MainApp();

	myApp.AddState<ShapeState>("ShapeState");
	
	myApp.Run(config);
	
	return 0;
}