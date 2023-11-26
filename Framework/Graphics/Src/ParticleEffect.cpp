#include "Precompiled.h"
#include "ParticleEffect.h"

#include "MeshBuffer.h"
#include "VertexTypes.h"
#include "RenderObject.h"
#include "Camera.h"

using namespace NEngine::Graphics;
using namespace NEngine;

void ParticleEffect::Initialize()
{
	std::filesystem::path filepath = "../../Assets/Shaders/DoParticle.fx";
	mVertexShader.Initialize<VertexPC>(filepath);
	mPixelShader.Initialize(filepath);
	mTransformBuffer.Initialize();
	mBlendState.Initailize(BlendState::Mode::AlphaBlend);
}

void ParticleEffect::Terminate()
{
	mBlendState.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void ParticleEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mTransformBuffer.BindVS(0);
	mBlendState.Set();
}

void ParticleEffect::End()
{
	mBlendState.ClearState();
}

void ParticleEffect::Render(const RenderObject& renderObject)
{
	auto matWorld = renderObject.transform.GetMatrix4();
	auto matView = mCamera->GetViewMatrix();
	auto matProj = mCamera->GetPerspectiveMatrix();
	auto matFinal = matWorld * matView * matProj;

	TransformData data;
	data.wvp = NMath::Transpose(matFinal);
	mTransformBuffer.Update(data);

	renderObject.meshBuffer.Render();
}

void ParticleEffect::DebugUI()
{
}

void ParticleEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
