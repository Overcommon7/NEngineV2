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
	if (ImGui::CollapsingHeader("ParticleSystem", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat3("SpawnPosition", &mInfo.spawnPosition.x);
		if (ImGui::DragFloat3("SpawnDirection", &mInfo.spawnDirection.x))
		{
			mInfo.spawnDirection = NMath::Normalize(mInfo.spawnDirection);
		}

		ImGui::DragInt("MinParticles", &mInfo.minParticlesPerEmit, 1.f);
		ImGui::DragInt("MaxParticles", &mInfo.maxParticlesPerEmit, 1.f, mInfo.minParticlesPerEmit);
		ImGui::DragFloat("MinTime", &mInfo.minTimeBetweenParticles, 0.1f);
		ImGui::DragFloat("maxTime", &mInfo.maxTimeBetweenParticles, 0.1f);
		ImGui::DragFloat("MinAngle", &mInfo.minSpawnAngle, 0.01f);
		ImGui::DragFloat("MaxAngle", &mInfo.maxSpawnAngle, 0.01f);
		ImGui::DragFloat("Lifetime", &mInfo.lifeTime, 0.5f);
		ImGui::ColorEdit4("StartColor", &mInfo.particleInfo.colorStart.r);
		ImGui::ColorEdit4("EndColor", &mInfo.particleInfo.colorEnd.r);
		ImGui::DragFloat3("StartScale", &mInfo.particleInfo.scaleStart.x);
		ImGui::DragFloat3("EndScale", &mInfo.particleInfo.scaleEnd.x);
		ImGui::DragFloat("MinSpeed", &mInfo.minSpeed);
		ImGui::DragFloat("MaxSpeed", &mInfo.maxSpeed);
    }
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

	float randFloat = (float)rand() / (float)RAND_MAX;
	float randAngle = mInfo.minSpawnAngle + ((mInfo.maxSpawnAngle - mInfo.minSpawnAngle) * randFloat);

	Vector3 rotAxis = (Dot(mInfo.spawnDirection, Vector3::YAxis) >= 0.99) ? Vector3::XAxis : Vector3::ZAxis;
	rotAxis = NMath::Normalize(NMath::Cross(rotAxis, mInfo.spawnDirection));
	auto matRot = Matrix4::RotationAxis(rotAxis, randAngle);
	auto spawnDirection = NMath::TransformNormal(mInfo.spawnDirection, matRot);

	randFloat = (float)rand() / (float)RAND_MAX;
	float speed = mInfo.minSpeed + ((mInfo.maxSpeed - mInfo.minSpeed) * randFloat);

	particle.Activate(mInfo.particleInfo, mInfo.spawnPosition, spawnDirection * speed);
}
