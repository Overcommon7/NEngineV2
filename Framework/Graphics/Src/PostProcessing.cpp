#include "Precompiled.h"
#include "PostProcessing.h"

#include "Camera.h"
#include "GraphicsSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace NEngine;

namespace
{
	inline constexpr const char* modeNames[] = {
			 "None",
			 "Monochrome",
			 "Invert",
			 "Mirror",
			 "Blur",
			 "Combine2",
			 "MotionBlur"
	};
}

void NEngine::Graphics::PostProcessingEffect::Initialize(const std::filesystem::path& filepath)
{
	mVertexShader.Initialize<VertexPX>(filepath);
	mPixelShader.Initialize(filepath);

	mSampler.Initialize(Sampler::Filter::Point, Sampler::AddressMode::Wrap);
	mPostProcessBuffer.Initialize();
}

void NEngine::Graphics::PostProcessingEffect::Terminate()
{
	mSampler.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
	mPostProcessBuffer.Terminate();
}

void NEngine::Graphics::PostProcessingEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();
	mSampler.BindPS(0);
	
	for (uint32_t i = 0; i < mTextures.size(); ++i)
	{
		if (mTextures[i])
		{
			mTextures[i]->BindPS(i);
		}
	}
	PostPrcocessData data;
	data.mode = (int)mMode;

	switch (mMode)
	{
	case Mode::None:
		break;
	case Mode::Monochrome:
		break;
	case Mode::Invert:

		break;
	case Mode::Mirror:
		data.params0 = params.mirrorScale.x;
		data.params1 = params.mirrorScale.y;
		break;
	case Mode::Blur:
	{
		auto gs = GraphicsSystem::Get();
		const auto screenWidth = gs->GetBackBufferHeight();
		const auto screenHeight = gs->GetBackBufferWidth();

		data.params0 = params.blurStrength / screenWidth;
		data.params1 = params.blurStrength / screenHeight;
	}	
	break;
	case Mode::Combine2:

		break;
	case Mode::MotionBlur:
		break;
	}


	mPostProcessBuffer.Update(data);
	mPostProcessBuffer.BindPS(0);
}

void NEngine::Graphics::PostProcessingEffect::End()
{
	for (uint32_t i = 0; i < mTextures.size(); ++i)
		Texture::UnbindPS(i);
}

void NEngine::Graphics::PostProcessingEffect::Render(const RenderObject& renderObject)
{
	renderObject.meshBuffer.Render();
}

void NEngine::Graphics::PostProcessingEffect::SetTexture(const Texture* texture, uint32_t slot)
{
	ASSERT(slot < mTextures.size(), "PostProcessingEffect: invalid slot index");
	mTextures[slot] = texture;
}

void NEngine::Graphics::PostProcessingEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int currentMode = (int)mMode;
		if (ImGui::Combo("Mode", &currentMode, modeNames, (int)std::size(modeNames)))
		{
			mMode = (Mode)currentMode;
		}
		if (mMode == Mode::Mirror)
			ImGui::DragFloat2("MirrorScale", &params.mirrorScale.x, 0.1f, -5, 5);
		else if (mMode == Mode::Blur)
			ImGui::DragFloat("BlurStrength", &params.blurStrength, 0.1f, 0, 10);

	}
}
