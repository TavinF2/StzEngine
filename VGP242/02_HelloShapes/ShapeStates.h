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
	
	using Vertices = std::vector<StzEngine::Graphics::VertexPC>;
	Vertices mVertices;

	StzEngine::Graphics::MeshBuffer mMeshBuffer;
	StzEngine::Graphics::VertexShader mVertexShader;
	StzEngine::Graphics::PixelShader mPixelShader;

};


class QuadState : public ShapeState
{
public: 
	void Update(float deltaTime) override;

protected:
	void CreateShape() override;

};

class TriForceState : public ShapeState
{
	public:
		void Update(float deltaTime) override;
protected:
	void CreateShape() override;
};

class HeartState : public ShapeState
{
	public:
		void Update(float deltaTime) override;
protected:
	void CreateShape() override;
};
