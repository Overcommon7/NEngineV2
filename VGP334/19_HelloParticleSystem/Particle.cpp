#include "Particle.h"

void Particle::Initialize()
{
	mMesh = MeshBuilder::CreateSpherePC(10.f, 10.f, 0.5f);
	mParticle.meshBuffer.Initialize(nullptr, sizeof(VertexPC), mMesh.vertices.size(), mMesh.indices.data(), mMesh.indices.size());

	mShape.InitializeEmpty();
	mRigidbody.Initialize(mParticle.transform, mShape, 0.1f);
}

void Particle::Terminate()
{
	mRigidbody.Terminate();
	mShape.Terminate();
	mParticle.Terminate();
}

void Particle::Activate(const ParticleInfo& info, const Vector3& pos, const Vector3& velocity)
{
	mLifeTime = 0;
	mLifeTimeDuration = info.lifetime;
	mIsActive = true;
	mInfo = info;
	mRigidbody.SetPosition(pos);
	mRigidbody.SetVelocity(velocity);
}

void Particle::Update(float deltaTime)
{
	if (!mIsActive) return;

	mLifeTime += deltaTime;
	float t = std::clamp(mLifeTime / mLifeTimeDuration, 0.f, 1.f);

	Color currentColor = Lerp(mInfo.colorStart, mInfo.colorEnd, t);
	mParticle.transform.scale = Lerp(mInfo.scaleStart, mInfo.scaleEnd, t);

	for (int i = 0; i < mMesh.vertices.size(); ++i)
	{
		mMesh.vertices[i].color = currentColor;
	}

	mParticle.meshBuffer.Update(mMesh.vertices.data(), mMesh.vertices.size());
}
