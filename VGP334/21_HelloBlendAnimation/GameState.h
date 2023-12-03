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

	enum class AnimationClipName
	{
		WalkingFowards = 1,
		WalkingBackwards = 2,
		StrafeRight = 3,
		StrafeLeft = 4
	};

protected:

	ModelId mId;
	Camera mCamera;
	DirectionalLight mDirectionalLight;
	StandardEffect mStandardEffect;
	RenderGroup mCharacter;
	RenderObject mGround;
	RenderTarget mRenderTarget;
	Animator mAnimator;
	

	bool mDrawSkeleton = false;
};