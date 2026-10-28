#include "GameState.h"

using namespace StzEngine;
using namespace StzEngine::Graphics;
using namespace StzEngine::Input;


void GameState::Initialize()
{
	mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
	mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

	mDirectionalLight.direction = Math::Normalize({ 1.0f, -1.0f, 1.0f });
	mDirectionalLight.ambient = { 0.4f, 0.4f, 0.4f, 1.0f };
	mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
	mDirectionalLight.specular = { 0.9f, 0.9f, 0.9f, 1.0f };

	Mesh mesh = MeshBuilder::CreateSphere(20, 20, 1);
	mRenderObject.meshBuffer.Initialize(mesh);
	TextureManager* tm = TextureManager::Get();
	mRenderObject.diffuseMapId = tm->LoadTexture("earth.jpg");
	mRenderObject.specMapId = tm->LoadTexture("earth_spec.jpg");
	mRenderObject.normMapId = tm->LoadTexture("earth_normal.jpg");
	mRenderObject.bumpMapId = tm->LoadTexture("earth_bump.jpg");

	//setup effect
	std::filesystem::path shaderPath = "../../Assets/Shaders/Standard.fx";
	mStandardEffect.Initialize(shaderPath);
	mStandardEffect.SetCamera(mCamera);
	mStandardEffect.SetDirectionalLight(mDirectionalLight);


	//set up cel effect

	shaderPath = L"../../Assets/Shaders/CelShader.fx";
	mStandardEffectCel.Initialize(shaderPath);
	mStandardEffectCel.SetCamera(mCamera);
	mStandardEffectCel.SetDirectionalLight(mDirectionalLight);

	//initialize render target

	const uint32_t size = 512;
	mRenderTarget.Initialize(size, size, RenderTarget::Format::RGBA_U32);

}

void GameState::Terminate()
{
	mRenderTarget.Terminate();
	mStandardEffectCel.Terminate();
	mStandardEffect.Terminate();
	mRenderObject.Terminate();
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

void GameState::Render()
{
	
	SimpleDraw::AddGroundPlane(20, Colors::LightGray);
	SimpleDraw::Render(mCamera);

	if (mUseCelShading)
	{
		mStandardEffectCel.Begin();
		mStandardEffectCel.Render(mRenderObject);
		mStandardEffectCel.End();

	}
	else
	{
		mStandardEffect.Begin();
		mStandardEffect.Render(mRenderObject);
		mStandardEffect.End();

	}
	// render target render
	mRenderTarget.BeginRender(); // tell gpu to render here
	mStandardEffect.Begin();
		mStandardEffect.Render(mRenderObject);
	mStandardEffect.End();
	mRenderTarget.EndRender();// tell gpu to render to window again
}


void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mStandardEffect.DebugUI();
	if (ImGui::CollapsingHeader("Light"))
	{

		if (ImGui::DragFloat3("Light Direction", &mDirectionalLight.direction.x, 0.01f))
		{
			mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
		}
		ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
		ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
		ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.specular.r);	

	}
	if (ImGui::CollapsingHeader("Material"))
	{
		ImGui::ColorEdit4("Emissive##Material", &mRenderObject.material.emissive.r);
		ImGui::ColorEdit4("Ambient##Material", &mRenderObject.material.ambient.r);
		ImGui::ColorEdit4("Diffuse##Material", &mRenderObject.material.diffuse.r);
		ImGui::ColorEdit4("Specular##Material", &mRenderObject.material.specular.r);
		ImGui::DragFloat("Shininess##Material", &mRenderObject.material.shininess, 0.01f, 0.0f, 1000.0f);
	}

	ImGui::Image(mRenderTarget.GetRawData(), { 128, 128 }, { 0, 0 }, { 1,1 }, { 1,1,1,1 }, { 1,1,1,1 });

	ImGui::Checkbox("UseCelShader", &mUseCelShading);
		if (mUseCelShading)
		{
			mStandardEffectCel.DebugUI();
		}
		else
		{
			mStandardEffect.DebugUI();
		}

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
