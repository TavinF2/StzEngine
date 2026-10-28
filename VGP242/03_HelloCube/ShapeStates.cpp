#include "ShapeStates.h"



using namespace StzEngine;
using namespace StzEngine::Graphics;
using namespace StzEngine::Input;

void ShapeState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });


	CreateShape();
	mMeshBuffer.Initialize(mVertices.data(), sizeof(VertexPC), (uint32_t)mVertices.size());
	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTransform.fx";
	mVertexShader.Initialize<VertexPC>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
}

void ShapeState::Terminate()
{
	mVertices.clear();
	mConstantBuffer.Terminate();
	mMeshBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
}

void ShapeState::Update(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;
	if(input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if(input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if(input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if(input->IsKeyDown(KeyCode::D))
	{
		mCamera.Strafe(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::E))
	{
		mCamera.Rise(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::Q))
	{
		mCamera.Rise(-moveSpeed * deltaTime);
	}

	if(input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}
}

void ShapeState::Render()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mConstantBuffer.BindVS(0);

	Math::Matrix4 matWorld = Math::Matrix4::Identity;
	Math::Matrix4 matView = mCamera.GetViewMatrix();
	Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
	Math::Matrix4 wvp = matWorld * matView * matProj;
	wvp = Math::Transpose(wvp);
	mConstantBuffer.Update(&wvp);


	mMeshBuffer.Render();
}

void ShapeState::CreateShape() // Creating a cube 
{
	// front
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::Red} });
	mVertices.push_back({ { 0.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::Green} });
	mVertices.push_back({ { 1.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::Blue} });

	mVertices.push_back({ { 1.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::HotPink} });
	mVertices.push_back({ { 1.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::Purple} });
	mVertices.push_back({ { 0.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::SeaGreen} });

	// back
	mVertices.push_back({ { 0.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::ForestGreen} });
	mVertices.push_back({ { 1.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::IndianRed} });
	mVertices.push_back({ { 0.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::BlueViolet} });

	mVertices.push_back({ { 1.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::DarkOliveGreen} });
	mVertices.push_back({ { 0.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::DarkSlateBlue} });
	mVertices.push_back({ { 1.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::OrangeRed} });

	// top
	mVertices.push_back({ { 0.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::LawnGreen} });
	mVertices.push_back({ { 0.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::LightSkyBlue} });
	mVertices.push_back({ { 1.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::OrangeRed} });

	mVertices.push_back({ { 1.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::LimeGreen} });
	mVertices.push_back({ { 1.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::MediumVioletRed} });
	mVertices.push_back({ { 0.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::PowderBlue} });

	// bottom
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::Yellow} });
	mVertices.push_back({ { 1.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::RoyalBlue} });
	mVertices.push_back({ { 0.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::DarkGreen} });

	mVertices.push_back({ { 1.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::DarkOrange} });
	mVertices.push_back({ { 0.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::MediumSeaGreen} });
	mVertices.push_back({ { 1.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::DarkSlateBlue} });

	// left
	mVertices.push_back({ { 0.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::PaleGreen} });
	mVertices.push_back({ { 0.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::PaleGoldenrod} });
	mVertices.push_back({ { 0.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::LightSteelBlue} });

	mVertices.push_back({ { 0.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::PowderBlue} });
	mVertices.push_back({ { 0.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::PaleGreen} });
	mVertices.push_back({ { 0.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::LightSteelBlue} });
	// right
	mVertices.push_back({ { 1.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::OrangeRed} });
	mVertices.push_back({ { 1.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::IndianRed} });
	mVertices.push_back({ { 1.0f, 0.0f, 0.0f }, {StzEngine::Graphics::Colors::BlueViolet} });

	mVertices.push_back({ { 1.0f, 1.0f, 1.0f }, {StzEngine::Graphics::Colors::Green} });
	mVertices.push_back({ { 1.0f, 0.0f, 1.0f }, {StzEngine::Graphics::Colors::Red} });
	mVertices.push_back({ { 1.0f, 1.0f, 0.0f }, {StzEngine::Graphics::Colors::Red} });
	

}
