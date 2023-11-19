#pragma once

#include "Inc/NEngine.h"

using namespace NEngine;
using namespace NEngine::Graphics;
using namespace NEngine::Physics;
using namespace NEngine::NMath;

struct ParticleInfo
{
	float lifetime = 0.0f;
	
	Color colorStart = Colors::White;
	Color colorEnd = Colors::White;

	Vector3 scaleStart = Vector3::One;
	Vector3 scaleEnd = Vector3::One;
};


class Particle
{
public:
	void Initialize();
	void Terminate();

	void Activate(const ParticleInfo& info, const Vector3& pos, const Vector3& velocity);
	void Update(float deltaTime);

	template<class Effect>
	void Render(Effect& effect)
	{
		if (mIsActive)
			effect.Render(mParticle);
	}

private:
	RenderObject mParticle;
	MeshPC mMesh;
	Rigidbody mRigidbody;
	CollisionShape mShape;

	bool mIsActive = false;
	float mLifeTime = 0.f;
	float mLifeTimeDuration = 0.0f;
	ParticleInfo mInfo;
};
