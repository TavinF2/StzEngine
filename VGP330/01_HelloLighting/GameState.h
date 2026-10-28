#pragma once

#include "StzEngine/Inc/StzEngine.h"


class GameState : public StzEngine::AppState {
	public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	void UpdateCamera(float deltaTime);


	StzEngine::Graphics::Camera mCamera;
	StzEngine::Graphics::DirectionalLight mDirectionalLight;
	StzEngine::Graphics::StandardEffect mStandardEffect;
	StzEngine::Graphics::StandardEffect mStandardEffectCel;
	StzEngine::Graphics::RenderTarget mRenderTarget;

	StzEngine::Graphics::RenderObject mRenderObject;
	bool mUseCelShading = false;
	
};