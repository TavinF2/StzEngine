#include "GameState.h"

using namespace StzEngine;
using namespace StzEngine::Graphics;
using namespace StzEngine::Input;

namespace {
	enum class PlanetType
	{
		Sun,
		Mercury,
		Venus,
		Earth,
		Moon,
		Mars,
		Jupiter,
		Saturn,
		Uranus,
		Neptune,
		Pluto,
		None
	};
}

const char* gPlanetNames[] = { "Sun", "Mercury", "Venus", "Earth", "Moon"
, "Mars", "Jupiter", "Saturn", "Uranus", "Neptune", "Pluto", "FreeCamera"};
PlanetType gCurrentPlanet = PlanetType::Sun;
bool gShowRings = false;
float gOrbitSpeedMultiplier = 0.2f;
float gRotationMultiplier = 1.0f;

Math::Vector3 GetMatrixPos(const Math::Matrix4& matrix)
{
	return { matrix._41, matrix._42, matrix._43 };
}

void InitializeRenderObject(RenderObject& renderObject, const MeshPX& mesh, std::filesystem::path filePath)
{
	renderObject.mesh = mesh;
	renderObject.meshBuffer.Initialize(mesh);
	renderObject.textureId = TextureManager::Get()->LoadTexture(filePath);
	renderObject.matWorld = Math::Matrix4::Identity;
}

void TerminateRenderObject(RenderObject& renderObject)
{
	TextureManager::Get()->ReleaseTexture(renderObject.textureId);
	renderObject.meshBuffer.Terminate();
}

void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
	mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

	std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
	mVertexShader.Initialize<VertexPX>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mConstantBuffer.Initialize(sizeof(Math::Matrix4));
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);

	InitializeRenderObject(mSpaceSphere,
		MeshBuilder::CreateSkySpherePX(64, 64, 500.0f),
		L"skysphere/highResSpace.jpg");

	CreateShape();

}

void GameState::Terminate()
{
	TerminateRenderObject(mSpaceSphere);
	TerminateRenderObject(mSun.renderObject);
	TerminateRenderObject(mMercury.renderObject);
	TerminateRenderObject(mVenus.renderObject);
	TerminateRenderObject(mEarth.renderObject);
	TerminateRenderObject(mMoon.renderObject);
	TerminateRenderObject(mMars.renderObject);
	TerminateRenderObject(mJupiter.renderObject);
	TerminateRenderObject(mSaturn.renderObject);
	TerminateRenderObject(mUranus.renderObject);
	TerminateRenderObject(mNeptune.renderObject);
	TerminateRenderObject(mPluto.renderObject);

	mConstantBuffer.Terminate();
	mVertexShader.Terminate();
	mPixelShader.Terminate();
	mSampler.Terminate();
}

void GameState::Update(float deltaTime)
{
	UpdatePlanet(mSun, deltaTime);

	const Math::Matrix4 sunCenter = Math::Matrix4::Translation(GetMatrixPos(mSun.renderObject.matWorld));

	mMercury.centerObject = sunCenter;
	UpdatePlanet(mMercury, deltaTime);

	mVenus.centerObject = sunCenter;
	UpdatePlanet(mVenus, deltaTime);

	mEarth.centerObject = sunCenter;
	UpdatePlanet(mEarth, deltaTime);

	mMoon.centerObject = Math::Matrix4::Translation(GetMatrixPos(mEarth.renderObject.matWorld));
	UpdatePlanet(mMoon, deltaTime);

	mMars.centerObject = sunCenter;
	UpdatePlanet(mMars, deltaTime);

	mJupiter.centerObject = sunCenter;
	UpdatePlanet(mJupiter, deltaTime);

	mSaturn.centerObject = sunCenter;
	UpdatePlanet(mSaturn, deltaTime);

	mUranus.centerObject = sunCenter;
	UpdatePlanet(mUranus, deltaTime);

	mNeptune.centerObject = sunCenter;
	UpdatePlanet(mNeptune, deltaTime);

	mPluto.centerObject = sunCenter;
	UpdatePlanet(mPluto, deltaTime);

	UpdateCamera(deltaTime);

	mSpaceSphere.matWorld = Math::Matrix4::Translation(mCamera.GetPosition());
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
	mVertexShader.Bind();
	mPixelShader.Bind();
	mConstantBuffer.BindVS(0);
	mSampler.BindPS(0);

	const Math::Matrix4 matView = mCamera.GetViewMatrix();
	const Math::Matrix4 matProj = mCamera.GetProjectionMatrix();

	auto renderObject = [&](const RenderObject& renderObject)
		{
			Math::Matrix4 wvp = renderObject.matWorld * matView * matProj;
			wvp = Math::Transpose(wvp);
			mConstantBuffer.Update(&wvp);

			TextureManager::Get()->BindPS(renderObject.textureId, 0);
			renderObject.meshBuffer.Render();
		};

	renderObject(mSpaceSphere);
	renderObject(mSun.renderObject);
	renderObject(mMercury.renderObject);
	renderObject(mVenus.renderObject);
	renderObject(mEarth.renderObject);
	renderObject(mMoon.renderObject);
	renderObject(mMars.renderObject);
	renderObject(mJupiter.renderObject);
	renderObject(mSaturn.renderObject);
	renderObject(mUranus.renderObject);
	renderObject(mNeptune.renderObject);
	renderObject(mPluto.renderObject);

	if (gShowRings)
	{
		const Math::Vector3 sunPosition = GetMatrixPos(mSun.renderObject.matWorld);
		const Math::Vector3 earthPosition = GetMatrixPos(mEarth.renderObject.matWorld);

		SimpleDraw::AddGroundCircle(128, mMercury.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mVenus.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mEarth.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(64, mMoon.distanceFromCenter, Colors::White, earthPosition);
		SimpleDraw::AddGroundCircle(128, mMars.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mJupiter.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mSaturn.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mUranus.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mNeptune.distanceFromCenter, Colors::White, sunPosition);
		SimpleDraw::AddGroundCircle(128, mPluto.distanceFromCenter, Colors::White, sunPosition);
	}

	SimpleDraw::Render(mCamera);
}


void GameState::DebugUI()
{
	ImGui::Begin("Solar System", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Checkbox("Show Orbit Rings", &gShowRings);
	ImGui::DragFloat("Orbit Speed Multiplier", &gOrbitSpeedMultiplier, 0.01f, 0.0f, 20.0f);
	ImGui::DragFloat("Day Speed Multiplier", &gRotationMultiplier, 0.01f, 0.0f, 20.0f);

	int currentPlanet = static_cast<int>(gCurrentPlanet);
	if (ImGui::Combo("Camera Target", &currentPlanet, gPlanetNames, static_cast<int>(std::size(gPlanetNames))))
	{
		gCurrentPlanet = static_cast<PlanetType>(currentPlanet);
	}

	auto editPlanet = [](const char* name, Planet& planet)
		{
			if (ImGui::CollapsingHeader(name))
			{
				ImGui::PushID(name);
				ImGui::DragFloat("Orbit Distance", &planet.distanceFromCenter, 0.1f, 0.0f, 500.0f);
				ImGui::DragFloat("Orbit Speed", &planet.orbitTranslation, 0.01f, 0.0f, 20.0f);
				ImGui::DragFloat("Day Speed", &planet.rotationOnAxis, 0.01f, 0.0f, 20.0f);
				ImGui::PopID();
			}
		};

	editPlanet("Sun", mSun);
	editPlanet("Mercury", mMercury);
	editPlanet("Venus", mVenus);
	editPlanet("Earth", mEarth);
	editPlanet("Moon", mMoon);
	editPlanet("Mars", mMars);
	editPlanet("Jupiter", mJupiter);
	editPlanet("Saturn", mSaturn);
	editPlanet("Uranus", mUranus);
	editPlanet("Neptune", mNeptune);
	editPlanet("Pluto", mPluto);

	ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
	Planet* targetPlanet = nullptr;

	switch (gCurrentPlanet)
	{
	case PlanetType::Sun: targetPlanet = &mSun; break;
	case PlanetType::Mercury: targetPlanet = &mMercury; break;
	case PlanetType::Venus: targetPlanet = &mVenus; break;
	case PlanetType::Earth: targetPlanet = &mEarth; break;
	case PlanetType::Moon: targetPlanet = &mMoon; break;
	case PlanetType::Mars: targetPlanet = &mMars; break;
	case PlanetType::Jupiter: targetPlanet = &mJupiter; break;
	case PlanetType::Saturn: targetPlanet = &mSaturn; break;
	case PlanetType::Uranus: targetPlanet = &mUranus; break;
	case PlanetType::Neptune: targetPlanet = &mNeptune; break;
	case PlanetType::Pluto: targetPlanet = &mPluto; break;
	case PlanetType::None: break;
	default: break;
	}

	if (targetPlanet != nullptr)
	{
		Math::Vector3 targetPosition = GetMatrixPos(targetPlanet->renderObject.matWorld);
		Math::Vector3 centerPosition = GetMatrixPos(targetPlanet->centerObject);
		Math::Vector3 orbitDirection = targetPosition - centerPosition;

		if (Math::MagnitudeSqr(orbitDirection) <= 0.0001f)
		{
			orbitDirection = { 0.0f, 0.0f, -1.0f };
		}
		else
		{
			orbitDirection = Math::Normalize(orbitDirection);
		}

		const float followDistance = Math::Max(3.0f, targetPlanet->radius * 6.0f);
		const float followHeight = Math::Max(1.5f, targetPlanet->radius * 2.0f);

		mCamera.SetPosition(targetPosition + (orbitDirection * followDistance) + Math::Vector3{ 0.0f, followHeight, 0.0f });
		mCamera.SetLookAt(targetPosition);
		return;
	}

	InputSystem* input = InputSystem::Get();
	const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 40.0f : 10.0f;
	const float turnSpeed = 0.3f;

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

void GameState::UpdatePlanet(Planet& planet, float deltaTime)
{
	planet.orbitAngle += planet.orbitTranslation * gOrbitSpeedMultiplier * deltaTime;
	planet.selfRotationAngle += planet.rotationOnAxis * gRotationMultiplier * deltaTime;

	const Math::Matrix4 matScale = Math::Matrix4::Scaling(planet.radius);
	const Math::Matrix4 matSelfRotation = Math::Matrix4::RotationY(planet.selfRotationAngle);
	const Math::Matrix4 matTranslation = Math::Matrix4::Translation(planet.distanceFromCenter, 0.0f, 0.0f);
	const Math::Matrix4 matOrbitRotation = Math::Matrix4::RotationY(planet.orbitAngle);

	planet.renderObject.matWorld =
		matScale *
		matSelfRotation *
		matTranslation *
		matOrbitRotation *
		planet.centerObject;
}

void GameState::CreateShape()
{
	const MeshPX sphereMesh = MeshBuilder::CreateSpherePX(32, 32, 1);

	auto initializePlanet = [&](Planet& planet, float radius, float distanceFromCenter, float orbitSpeed, float daySpeed, const wchar_t* texturePath)
		{
			planet.radius = radius;
			planet.centerObject = Math::Matrix4::Identity;
			planet.distanceFromCenter = distanceFromCenter;
			planet.orbitTranslation = orbitSpeed;
			planet.rotationOnAxis = daySpeed;
			planet.orbitAngle = 0.0f;
			planet.selfRotationAngle = 0.0f;
			InitializeRenderObject(planet.renderObject, sphereMesh, texturePath);
		};

	initializePlanet(mSun, 8.0f, 0.0f, 0.0f, 0.2f, L"sun.jpg");
	initializePlanet(mMercury, 1.0f, 14.0f, 4.8f, 2.0f, L"planets/mercury.jpg");
	initializePlanet(mVenus, 1.4f, 20.0f, 3.5f, 1.6f, L"planets/venus.jpg");
	initializePlanet(mEarth, 1.5f, 28.0f, 3.0f, 3.0f, L"planets/earth/earth.jpg");
	initializePlanet(mMoon, 0.45f, 3.5f, 8.0f, 2.0f, L"planets/pluto.jpg");
	initializePlanet(mMars, 1.2f, 36.0f, 2.4f, 2.4f, L"planets/mars.jpg");
	initializePlanet(mJupiter, 4.0f, 50.0f, 1.3f, 4.0f, L"planets/jupiter.jpg");
	initializePlanet(mSaturn, 3.5f, 66.0f, 1.0f, 3.4f, L"planets/saturn.jpg");
	initializePlanet(mUranus, 2.6f, 80.0f, 0.7f, 2.8f, L"planets/uranus.jpg");
	initializePlanet(mNeptune, 2.5f, 94.0f, 0.5f, 2.7f, L"planets/neptune.jpg");
	initializePlanet(mPluto, 0.8f, 108.0f, 0.35f, 1.8f, L"planets/pluto.jpg");

	mMercury.orbitAngle = 0.2f;
	mVenus.orbitAngle = 0.7f;
	mEarth.orbitAngle = 1.2f;
	mMars.orbitAngle = 1.7f;
	mJupiter.orbitAngle = 2.1f;
	mSaturn.orbitAngle = 2.6f;
	mUranus.orbitAngle = 3.0f;
	mNeptune.orbitAngle = 3.5f;
	mPluto.orbitAngle = 4.0f;
	mMoon.orbitAngle = 0.5f;

	UpdatePlanet(mSun, 0.0f);

	const Math::Matrix4 sunCenter = Math::Matrix4::Translation(GetMatrixPos(mSun.renderObject.matWorld));

	mMercury.centerObject = sunCenter;
	UpdatePlanet(mMercury, 0.0f);

	mVenus.centerObject = sunCenter;
	UpdatePlanet(mVenus, 0.0f);

	mEarth.centerObject = sunCenter;
	UpdatePlanet(mEarth, 0.0f);

	mMoon.centerObject = Math::Matrix4::Translation(GetMatrixPos(mEarth.renderObject.matWorld));
	UpdatePlanet(mMoon, 0.0f);

	mMars.centerObject = sunCenter;
	UpdatePlanet(mMars, 0.0f);

	mJupiter.centerObject = sunCenter;
	UpdatePlanet(mJupiter, 0.0f);

	mSaturn.centerObject = sunCenter;
	UpdatePlanet(mSaturn, 0.0f);

	mUranus.centerObject = sunCenter;
	UpdatePlanet(mUranus, 0.0f);

	mNeptune.centerObject = sunCenter;
	UpdatePlanet(mNeptune, 0.0f);

	mPluto.centerObject = sunCenter;
	UpdatePlanet(mPluto, 0.0f);
}