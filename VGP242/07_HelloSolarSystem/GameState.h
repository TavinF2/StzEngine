#pragma once

#include "StzEngine/Inc/StzEngine.h"

struct RenderObject
{
	StzEngine::Graphics::MeshPX mesh;
	StzEngine::Graphics::MeshBuffer meshBuffer;
	StzEngine::Math::Matrix4 matWorld = StzEngine::Math::Matrix4::Identity;
	StzEngine::Graphics::TextureId textureId;
};

struct Planet
{
	float radius;
	StzEngine::Math::Matrix4 centerObject = StzEngine::Math::Matrix4::Identity;
	float distanceFromCenter;
	float orbitTranslation;
	float rotationOnAxis;
	float orbitAngle = 0.0f;
	float selfRotationAngle = 0.0f;
	RenderObject renderObject;
};

class GameState : public StzEngine::AppState {
	public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCamera(float deltaTime);
	void UpdatePlanet(Planet& planet, float deltaTime);
	void CreateShape();

	StzEngine::Graphics::Camera mCamera;

	StzEngine::Graphics::VertexShader mVertexShader;
	StzEngine::Graphics::PixelShader mPixelShader;
	StzEngine::Graphics::Sampler mSampler;
	StzEngine::Graphics::ConstantBuffer mConstantBuffer;

	RenderObject mSpaceSphere;

	Planet mSun;
	Planet mMercury;
	Planet mVenus;
	Planet mEarth;
	Planet mMoon;
	Planet mMars;
	Planet mJupiter;
	Planet mSaturn;
	Planet mUranus;
	Planet mNeptune;
	Planet mPluto;
};