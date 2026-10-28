#include "ShapeStates.h"



using namespace StzEngine;
using namespace StzEngine::Graphics;
using namespace StzEngine::Input;

void ShapeState::Initialize()
{
	CreateShape();
	mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), (uint32_t)mVertices.size());
	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoColor.fx";
	mVertexShader.Initialize<VertexPC>(shaderPath);
	mPixelShader.Initialize(shaderPath);
}

void ShapeState::Terminate()
{
	mVertices.clear();
	mMeshBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void ShapeState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::UP))
	{
		MainApp().ChangeState("QuadState");
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::LEFT))
	{
		MainApp().ChangeState("TriForceState");
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::RIGHT))
	{
		MainApp().ChangeState("HeartState");
	}
}

void ShapeState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{
	mVertices.push_back({ {-0.5f, -0.5f, 0}, {StzEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {0.0f, 0.5f, 0}, {StzEngine::Graphics::Colors::Green} });
	mVertices.push_back({ {0.5f, -0.5f, 0}, {StzEngine::Graphics::Colors::Blue} });
}

void QuadState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
	{
		MainApp().ChangeState("ShapeState");
	}
	if(InputSystem::Get()->IsKeyPressed(KeyCode::LEFT))
	{
		MainApp().ChangeState("TriForceState");
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::RIGHT))
	{
		MainApp().ChangeState("HeartState");
	}
}

void QuadState::CreateShape()
{
	mVertices.push_back({ {-0.5f, -0.5f, 0}, {StzEngine::Graphics::Colors::Red} });
	mVertices.push_back({ {-0.5f, 0.5f, 0}, {StzEngine::Graphics::Colors::Green} });
	mVertices.push_back({ {0.5f, 0.5f, 0}, {StzEngine::Graphics::Colors::Blue} });

	mVertices.push_back({ {-0.5f, -0.5f, 0}, {StzEngine::Graphics::Colors::HotPink}});
	mVertices.push_back({ {0.5f, 0.5f, 0}, {StzEngine::Graphics::Colors::Blue} });
	mVertices.push_back({ {0.5f, -0.5f, 0}, {StzEngine::Graphics::Colors::ForestGreen} });
}

void TriForceState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
	{
		MainApp().ChangeState("ShapeState");
	}
}

void TriForceState::CreateShape()
{
	mVertices.push_back({ {  0.0f,  0.5f, 0.0f }, { StzEngine::Graphics::Colors::Green } });
	mVertices.push_back({ {  0.25f, 0.0f, 0.0f }, { StzEngine::Graphics::Colors::Green } });
	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, { StzEngine::Graphics::Colors::Green } });

	mVertices.push_back({ { -0.5f, -0.5f, 0.0f }, { StzEngine::Graphics::Colors::Red } });
	mVertices.push_back({ { -0.25f, 0.0f, 0.0f }, { StzEngine::Graphics::Colors::Red } });
	mVertices.push_back({ {  0.0f, -0.5f, 0.0f }, { StzEngine::Graphics::Colors::Red } });

	mVertices.push_back({ {  0.0f, -0.5f, 0.0f }, { StzEngine::Graphics::Colors::Blue } });
	mVertices.push_back({ {  0.25f, 0.0f, 0.0f }, { StzEngine::Graphics::Colors::Blue } });
	mVertices.push_back({ {  0.5f, -0.5f, 0.0f }, { StzEngine::Graphics::Colors::Blue } });
}

void HeartState::Update(float deltaTime)
{
	if (InputSystem::Get()->IsKeyPressed(KeyCode::DOWN))
	{
		MainApp().ChangeState("ShapeState");
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::RIGHT))
	{
		MainApp().ChangeState("QuadState");
	}
	if (InputSystem::Get()->IsKeyPressed(KeyCode::LEFT))
	{
		MainApp().ChangeState("TriForceState");
	}
}

void HeartState::CreateShape()
{
	// Top-left lobe
	mVertices.push_back({ { -0.25f,  0.5f, 0.0f }, { StzEngine::Graphics::Colors::HotPink } });
	mVertices.push_back({ {  0.0f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::HotPink } });
	mVertices.push_back({ { -0.5f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::HotPink } });

	// Top-right lobe
	mVertices.push_back({ {  0.25f,  0.5f, 0.0f }, { StzEngine::Graphics::Colors::BlueViolet } });
	mVertices.push_back({ {  0.5f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::BlueViolet } });
	mVertices.push_back({ {  0.0f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::BlueViolet } });

	mVertices.push_back({ { -0.5f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::Crimson } });
	mVertices.push_back({ {  0.0f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::Crimson } });
	mVertices.push_back({ {  0.0f,  -0.45f, 0.0f }, { StzEngine::Graphics::Colors::Crimson } });

	mVertices.push_back({ {  0.0f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::Red } });
	mVertices.push_back({ {  0.5f,   0.25f, 0.0f }, { StzEngine::Graphics::Colors::Red } });
	mVertices.push_back({ {  0.0f,  -0.45f, 0.0f }, { StzEngine::Graphics::Colors::Red } });


}