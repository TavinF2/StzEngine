#include "Precompiled.h"
#include "StzEngine.h"

StzEngine::App& StzEngine::MainApp()
{
	static App sApp;
	return sApp;
}