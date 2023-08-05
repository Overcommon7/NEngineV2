#pragma once
#include "Inc/AppState.h"
#include "Inc/WNTRengine.h"

using namespace NEngine;
using namespace NEngine::WNTRmath;
using namespace NEngine::Graphics;

class GameState : public AppState
{
public:
	void Initialize();
	void Terminate();
	void Render();
	void Update(float deltaTime);

protected:

	Camera mCamera;
	ConstantBuffer mConstantBuffer;
	MeshBuffer mMeshBuffer;
	VertexShader mVertexBuffer;
	PixelShader mPixelShader;
	
	Texture mDiffuseTexture;
	Sampler mSampler;
};


