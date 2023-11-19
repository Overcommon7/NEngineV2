#pragma once

#include "Particle.h"

struct ParticleSystemInfo
{
	ParticleInfo particleInfo;
	Vector3 spawnPsosition;
	Vector3 spawnDirection;

	float spawnDelay;
	int minParticlesPerEmit = 0;
	int maxParticlesPerEmit = 0;
	float minTimeBetweenParticles = 0.f;
	float maxTimeBetweenParticles = 0.f;
	float minSpawnAngle = 0.f;
	float maxSpawnAngle = 0.f;
	float lifeTime = 0.f;
	int maxParticles = 0.f;
	int maxParticles = 100;
};

class ParticleSystem
{
public:
	void Initialize(const ParticleSystemInfo& info);
	void Terminate();
	void Update(float deltaTime);

	template<class Effect>
	void Render(Effect& effect)
	{
		if (mLifeTime <= 0.f) return;

		for (auto& particle : mParticles)
		{
			particle.Render(effect);
		}
	}

	void DebugUI();

private:

	void SpawnParticles();
	void SpawnParticle();

	vector<Particle> mParticles;
	int mNextAvailibleParticleIndex;
	float mNextSpawn = 0.f;
	float mLifeTime = 0.f;

	ParticleSystemInfo mInfo;
}; 


