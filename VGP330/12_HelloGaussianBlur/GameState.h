#pragma once

#include "Inc/AppState.h"
#include "Inc/NEngine.h"

using namespace NEngine;
using namespace NEngine::Graphics;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);
	void DebugUI();

protected:

	Camera mCamera;
	DirectionalLight mDirectionalLight;

	StandardEffect mStandardEffect;
	GaussianBlur mGaussianBlurEffect;
	PostProcessingEffect mPostProceessingEffect;

	RenderObject mSun;
	RenderObject mEarth;
	RenderObject mScreenQuad;

	RenderTarget mBaseRenderTarget;
	RenderTarget mBloomRenderTarget;

	float mEarthOffset = 8.0f;
	float mEarthRotation = 0.0f;
	float mEarthRevolution = 0.0f;
};


