#include "framework.h"
#include "ParticleManager.h"

ParticleManager::~ParticleManager()
{
	for (auto& particles : totalParticle) {
		for (auto particle : particles.second)
			delete particle;
	}
	totalParticle.clear();
}

void ParticleManager::Update()
{
	for (auto& particles : totalParticle)
		for (auto particle : particles.second)
			particle->Update();
}

void ParticleManager::Render()
{
	for (auto& particles : totalParticle)
		for (auto particle : particles.second)
			particle->Render();
}

void ParticleManager::Play(string key, Vector3 pos, Vector3 rot)
{
	if (totalParticle.count(key) == 0)
		return;

	for (auto particle : totalParticle[key]) {
		if (particle->IsActive())
			continue;

		particle->Play(pos, rot);
		return;
	}
}

void ParticleManager::Add(string key, string file, UINT poolSize)
{
	if (totalParticle.find(key) != totalParticle.end())
		return;

	Particles particles(poolSize);
	for (auto& particle : particles)
		particle = new ParticleSystem(file);

	totalParticle[key] = particles;
}
