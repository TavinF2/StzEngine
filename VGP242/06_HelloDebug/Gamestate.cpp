#include "GameState.h"

using namespace StzEngine;
using namespace StzEngine::Graphics;
using namespace StzEngine::Input;

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
	UpdateCamera(deltaTime);
}
enum class Shape
{
	None,
	AABB,
	AABBFilled,
	Sphere,
	GroundPlane,
	GroundCircle,
	Transform
};
const char* gShapeNames[] = { "None", "AABB", "AABBFilled", "Sphere", "GroundPlane", "GroundCircle", "Transform" };
Shape gCurrentShape = Shape::None;
Color gShapeColor = Colors::White;
float gPlaneSize = 10.0f;
float gGroundCircleRadius = 5.0f;
float gSphereRadius = 1.0f;
void GameState::Render()
{
	switch (gCurrentShape)
	{
	case Shape::None: break;
	case Shape::GroundPlane:
	{
		SimpleDraw::AddGroundPlane(gPlaneSize, gShapeColor);
	}
	break;
	case Shape::GroundCircle:
	{// AddGroundCircle(uint32_t slices, float radius, const Color& color, const Math::Vector3& origin)
		SimpleDraw::AddGroundCircle(32, gGroundCircleRadius, gShapeColor, Math::Vector3::Zero);
	}
	break;
	case Shape::Sphere:
	{
		SimpleDraw::AddSphere(32, 32, gSphereRadius, gShapeColor, Math::Vector3::Zero);
	}
	break;
	default:
		break;
	}
	SimpleDraw::Render(mCamera);
}


void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::ColorEdit4("Shape Color", &gShapeColor.r);
	int currentShape = (int)gCurrentShape;
	if(ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
	{
		gCurrentShape = (Shape)currentShape;
	}
	ImGui::DragFloat("Plane Size", &gPlaneSize, 1.0f, 0.0f, 10000.0f);
	ImGui::DragFloat("Ground Circle Radius", &gGroundCircleRadius, 1.0f, 0.0f, 10000.0f);
	ImGui::DragFloat("Sphere Radius", &gSphereRadius, 0.1f, 0.0f, 10000.0f);
	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
	const float turnSpeed = 0.1f;
	if (input->IsKeyDown(KeyCode::W))
	{
		mCamera.Walk(moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::S))
	{
		mCamera.Walk(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::A))
	{
		mCamera.Strafe(-moveSpeed * deltaTime);
	}
	if (input->IsKeyDown(KeyCode::D))
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

	if (input->IsMouseDown(MouseButton::RBUTTON))
	{
		mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
		mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
	}

}