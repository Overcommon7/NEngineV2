#include "Precompiled.h"
#include "ShadowEffect.h"

#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);
	std::filesystem::path shaderFile = "../../Assets/Shaders/Shadows.fx";

	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTrasformBuffer.Initialize();
	constexpr const uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mTrasformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ShadowEffect::Begin()
{
	UpadateLightCamera();

	mVertexShader.Bind();
	mPixelShader.Bind();
	mTrasformBuffer.BindVS(0);

	mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
	mDepthMapRenderTarget.EndRender();

}

void ShadowEffect::Render(const RenderObject& renderObject)
{

	const auto& matWorld = renderObject.transform.GetMatrix4();
	const auto& matView = mLightCamera.GetViewMatrix();
	const auto& matProj = mLightCamera.GetProjectionMatrix();

	TransformData data;
	data.wvp = NMath::Transpose(matWorld * matView * matProj);
	mTrasformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}

void ShadowEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("ShadowEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::Text("DepthMap");
		ImGui::Image(
			mDepthMapRenderTarget.GetRawData(),
			{ 144, 144 },
			{ 0, 0 },
			{ 1, 1 },
			{ 1, 1, 1, 1 },
			{ 1, 1, 1, 1 });
		ImGui::DragFloat("Size", &mSize, 1.0f, 1.0f, 1000.f);
	}
}

void ShadowEffect::UpadateLightCamera()
{
	ASSERT(mDirectionalLight != nullptr, "ShadowEffect: no light set!");


	const NMath::Vector3& direction = mDirectionalLight->direction;
	mLightCamera.SetDirection(direction);
	mLightCamera.SetPosition(mFocusPosition - (direction * 1000.f));
	mLightCamera.SetNearPlane(1.f);
	mLightCamera.SetFarPlane(2000.f);
	mLightCamera.SetSize(mSize, mSize);
}
