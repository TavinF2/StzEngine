#pragma once
#include <StzEngine/inc/StzEngine.h>

class ShapeState : public StzEngine::AppState
{
public:
	void Initialize() override;
	void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;

protected:
	virtual void CreateShape();
	
	StzEngine::Graphics::MeshPC mMesh;

	StzEngine::Graphics::MeshBuffer mMeshBuffer;
	StzEngine::Graphics::VertexShader mVertexShader;
	StzEngine::Graphics::PixelShader mPixelShader;

	StzEngine::Graphics::Camera mCamera;
	StzEngine::Graphics::ConstantBuffer mConstantBuffer;

};

