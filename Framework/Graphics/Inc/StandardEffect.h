#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Sampler.h"

namespace StzEngine::Graphics
{
	class Camera;
	class RenderObject;


	class StandardEffect final
	{
	public:
		void Initialize(const std::filesystem::path& shaderPath);
		void Terminate();
		
		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void DebugUI();

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);


	private:
		struct TransformData
		{
			Math::Matrix4 wvp; // world view projection matric for ndc space
			Math::Matrix4 world; // world location
			Math::Vector3 viewPosition; // position of camera
			float padding = 0.0f; // padding to make the struct 16-byte aligned
		};

		struct SettingsData
		{
			int useDiffuseMap = 1;
			int useSpecMap = 1;
			int useNormMap = 1;
			int useBumpMap = 1;
			float bumpWeight = 0.1f;
			float padding[3] = { 0.0f };
		};
		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		using MaterialBuffer = TypedConstantBuffer<Material>;
		MaterialBuffer mMaterialBuffer;

		using SettingsBuffer = TypedConstantBuffer<SettingsData>;
		SettingsBuffer mSettingsBuffer;
		

		VertexShader mVertexShader;
		PixelShader mPixelShader;
		Sampler mSampler;

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
		SettingsData mSettingsData;
		
	};
}