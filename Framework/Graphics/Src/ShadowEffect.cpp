#include "Precompiled.h"
#include "ShadowEffect.h"

#include "AnimationUtil.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NEngine;
using namespace NEngine::Graphics;

inline static constexpr size_t MaxBoneCount = 256;

void ShadowEffect::Initialize()
{
	mLightCamera.SetMode(Camera::ProjectionMode::Orthographic);
	std::filesystem::path shaderFile = "../../Assets/Shaders/Shadows.fx";

	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTrasformBuffer.Initialize();
	mBoneTransformBuffer.Initialize(MaxBoneCount * sizeof(NMath::Matrix4));
	constexpr const uint32_t depthMapResolution = 4096;
	mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution, Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
	mDepthMapRenderTarget.Terminate();
	mBoneTransformBuffer.Terminate();
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
	mBoneTransformBuffer.BindVS(1);

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
	
	if (renderObject.skeleton != nullptr)
	{
		AnimationUtil::BoneTransforms boneTransforms;
		AnimationUtil::ComputeBoneTransform(renderObject.modelId, boneTransforms, renderObject.animator);
		AnimationUtil::ApplyBoneOffsets(renderObject.modelId, boneTransforms);
		for (auto& transform : boneTransforms)
			transform = NMath::Transpose(transform);

		boneTransforms.resize(MaxBoneCount);
		mBoneTransformBuffer.Update(boneTransforms.data());
	}
	
		
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
