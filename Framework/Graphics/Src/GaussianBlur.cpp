#include "Precompiled.h"
#include "GaussianBlur.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NEngine;
using namespace NEngine::Graphics;

void GaussianBlur::Initalize()
{
	auto gs = GraphicsSystem::Get();

	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();
	mHorizantalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);
	mVerticalBlurRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

	std::filesystem::path shaderPath = "../../Assets/Shaders/GaussianBlur.fx";
	mVertexShader.Initialize<VertexPX>(shaderPath);
	mHorizontalBlurPixelShader.Initialize(shaderPath, "HorizantalBlurPS");
	mVerticalBlurPixelShader.Initialize(shaderPath, "VerticalBlurPS");

	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void GaussianBlur::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mVerticalBlurPixelShader.Terminate();
	mHorizontalBlurPixelShader.Terminate();
	mVerticalBlurRenderTarget.Terminate();
	mHorizantalBlurRenderTarget.Terminate();
}

void GaussianBlur::Begin()
{
	mVertexShader.Bind();

	auto gs = GraphicsSystem::Get();
	const auto screenWidth = gs->GetBackBufferWidth();
	const auto screenHeight = gs->GetBackBufferHeight();

	SettingsData data;
	data.screenWidth = screenWidth;
	data.screenHeight = screenHeight;
	data.multiplier = mBlurSaturation;
	
	mSettingsBuffer.Update(data);
	mSettingsBuffer.BindPS(0);

	mSampler.BindPS(0);
}

void GaussianBlur::End()
{
	auto gs = GraphicsSystem::Get();
	gs->ResetRenderTarget();
	gs->ResetViewport();
}

void GaussianBlur::Render(const RenderObject& renderObject)
{
	auto gs = GraphicsSystem::Get();
	mHorizantalBlurRenderTarget.BeginRender();
	mSourceTexture->BindPS(0);
	mHorizontalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mHorizantalBlurRenderTarget.EndRender();

	for (uint32_t i = 0; i < mBlurIterations; ++i)
	{
		mVerticalBlurRenderTarget.BeginRender();
		mHorizantalBlurRenderTarget.BindPS(0);
		mVerticalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mVerticalBlurRenderTarget.EndRender();


		mHorizantalBlurRenderTarget.BeginRender();
		mVerticalBlurRenderTarget.BindPS(0);
		mHorizontalBlurPixelShader.Bind();
		renderObject.meshBuffer.Render();
		Texture::UnbindPS(0);
		mHorizantalBlurRenderTarget.EndRender();
	}

	mVerticalBlurRenderTarget.BeginRender();
	mHorizantalBlurRenderTarget.BindPS(0);
	mVerticalBlurPixelShader.Bind();
	renderObject.meshBuffer.Render();
	Texture::UnbindPS(0);
	mVerticalBlurRenderTarget.EndRender();
}

void NEngine::Graphics::GaussianBlur::DebugUI()
{
	if (ImGui::CollapsingHeader("GaussianBlurEffect##", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int iterations = mBlurIterations;
		if (ImGui::DragInt("BlurIterations##", &iterations, 1, 1, 100))
			mBlurIterations = iterations;

		ImGui::DragFloat("BlurSaturation##", &mBlurSaturation, 0.001f, 1, 10);
		ImGui::Text("Horizontal");
		ImGui::Image(mHorizantalBlurRenderTarget.GetRawData(), { 144, 144 });
		ImGui::Text("Vertical");
		ImGui::Image(mVerticalBlurRenderTarget.GetRawData(), { 144, 144 });
	
	}
}
