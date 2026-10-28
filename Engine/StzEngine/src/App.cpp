#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

using namespace StzEngine;
using namespace StzEngine::Core;
using namespace StzEngine::Input;
using namespace StzEngine::Graphics;

void App::Run(const AppConfig& config)
{
	LOG("App Started");
	Window myWindow;
	myWindow.Initialize(GetModuleHandle(nullptr), config.appName, config.windowWidth, config.windowHeight);

	auto handle = myWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);
	GraphicsSystem::StaticInitialize(handle, config.fullscreen);
	TextureManager::StaticInitialize(L"../../Assets/Textures");
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize(config.maxVertexCount);

	ASSERT(mCurrentState != nullptr, "App: need an app state to run");
	mCurrentState->Initialize();
	InputSystem* input = InputSystem::Get();

	mRunning = true;
	while (mRunning == true)
	{
		myWindow.ProcessMessage();
		input->Update();
		if (myWindow.IsActive() == false || input->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}
		if (mNextState != nullptr )
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}
		float deltaTime = TimeUtil::GetDeltaTime();
#if define(_DEBUG)
		if (deltaTime > 0.5f)
		{
			LOG("App: really long Delta time skipping frame %f", deltaTime);
			continue;
		}
#endif 
		mCurrentState->Update(deltaTime);

		GraphicsSystem* gs = GraphicsSystem::Get();
		gs->BeginRender();
		mCurrentState->Render();
		DebugUI::BeginDraw();
			mCurrentState->DebugUI();
		DebugUI::EndDraw();
		gs->EndRender();
	}
	mCurrentState->Terminate();


	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	TextureManager::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	myWindow.Terminate();
	LOG("App Ended");
}
void App::Quit()
{
	mRunning = false;
}

void StzEngine::App::ChangeState(const std::string& stateName)
{
	//AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
	auto iter = mAppStates.find(stateName);
	if (iter != mAppStates.end())
	{
		mNextState = iter->second.get();
	}
}
