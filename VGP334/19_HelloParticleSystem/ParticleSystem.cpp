#include "ParticleSystem.h"

void ParticleSystem::Initialize(const ParticleSystemInfo& info)
{
	mInfo = info;
	mNextAvailibleParticleIndex = 0;
	mNextSpawn = info.spawnDelay;
	mLifeTime = info.lifeTime;
	mParticles.resize(info.maxParticles);

	for (auto& p : mParticles)
		p.Initialize();
}

void ParticleSystem::Terminate()
{
	for (auto& p : mParticles)
		p.Terminate();
}

void ParticleSystem::Update(float deltaTime)
{
	if (mLifeTime <= 0.f) return;

	mLifeTime -= deltaTime;
	mNextSpawn -= deltaTime;
	if (mNextSpawn <= 0.f)
	{
		SpawnParticles();
	}
	for (auto& p : mParticles)
		p.Update(deltaTime);
}

void ParticleSystem::DebugUI()
{

}

void ParticleSystem::SpawnParticles()
{
	int numParticles = NMath::Random(mInfo.maxParticlesPerEmit, mInfo.minParticlesPerEmit);
	for (int i = 0; i < numParticles; ++i)
		SpawnParticle();

	mNextSpawn = mInfo.spawnDelay;
}

void ParticleSystem::SpawnParticle()
{
	auto& particle = mParticles[mNextAvailibleParticleIndex++];
	mNextAvailibleParticleIndex %= mParticles.size();
}
