#include "Precompiled.h"
#include "StandardEffect.h"
#include "Camera.h"
#include "VertexTypes.h"
#include "RenderObject.h"

using namespace StzEngine;
using namespace StzEngine::Graphics;

void StzEngine::Graphics::StandardEffect::Initialize(const std::filesystem::path& shaderPath)
{
	//Initialize Buffers
	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();

	//Other rendering componenets
	mVertexShader.Initialize<Vertex>(shaderPath);
	mPixelShader.Initialize(shaderPath);
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void StzEngine::Graphics::StandardEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
}

void StzEngine::Graphics::StandardEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindVS(0);
	mSampler.BindPS(0);

	mTransformBuffer.BindVS(0);
	mLightBuffer.BindPS(1);
	mLightBuffer.BindVS(1);
	mMaterialBuffer.BindPS(2);
	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);
}

void StzEngine::Graphics::StandardEffect::End()
{
}

void StzEngine::Graphics::StandardEffect::Render(const RenderObject& renderObject)
{
	const Math::Matrix4 matWorld = renderObject.transform.GetMatrix();
	const Math::Matrix4 matView = mCamera->GetViewMatrix();
	const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
	const Math::Matrix4 matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = Math::Transpose(matFinal);
	data.world = Math::Transpose(matWorld);
	data.viewPosition = mCamera->GetPosition();
	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);

	SettingsData settings;
	settings.useDiffuseMap = (mSettingsData.useDiffuseMap > 0 && renderObject.diffuseMapId > 0) ? 1 : 0;
	settings.useSpecMap = (mSettingsData.useSpecMap > 0 && renderObject.specMapId > 0) ? 1 : 0;
	settings.useNormMap = (mSettingsData.useNormMap > 0 && renderObject.normMapId > 0) ? 1 : 0;
	settings.useBumpMap = (mSettingsData.useBumpMap > 0 && renderObject.bumpMapId > 0) ? 1 : 0;
	settings.bumpWeight = mSettingsData.bumpWeight;
	mSettingsBuffer.Update(settings);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMapId, 1);
	tm->BindPS(renderObject.normMapId, 2);
	tm->BindVS(renderObject.bumpMapId, 3);

	renderObject.meshBuffer.Render();
}

void StzEngine::Graphics::StandardEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("StandardEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useDiffuse = mSettingsData.useDiffuseMap > 0;
		if (ImGui::Checkbox("Use Diffuse Map", &useDiffuse))
		{
			mSettingsData.useDiffuseMap = useDiffuse ? 1 : 0;
		}
		bool useSpec = mSettingsData.useSpecMap > 0;
		if (ImGui::Checkbox("Use Specular Map", &useSpec))
		{
			mSettingsData.useSpecMap = useSpec ? 1 : 0;
		}
		bool useNorm = mSettingsData.useNormMap > 0;
		if (ImGui::Checkbox("Use Normal Map", &useNorm))
		{
			mSettingsData.useNormMap = useNorm ? 1 : 0;
		}
		bool useBump = mSettingsData.useBumpMap > 0;
		if (ImGui::Checkbox("Use Bump Map", &useBump))
		{
			mSettingsData.useBumpMap = useBump ? 1 : 0;
		}
		ImGui::DragFloat("Bump Weight", &mSettingsData.bumpWeight, 0.01f, 0.0f, 100.0f);	
	}
}

void StzEngine::Graphics::StandardEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}

void StzEngine::Graphics::StandardEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}
