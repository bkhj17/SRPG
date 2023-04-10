#include "framework.h"

Obstacle::Obstacle()
{
	model = new Model("Box01");
	model->Load();
	model->SetParent(this);
}

Obstacle::~Obstacle()
{
	delete model;
}

void Obstacle::Update()
{
	if (!Active())
		return;
	__super::Update();

	model->UpdateWorld();
}

void Obstacle::Render()
{
	if (!Active())
		return;

	model->Render();
}

void Obstacle::Damaged(int damage)
{
	__super::Damaged(damage);
	if (status.curHp <= 0) {
		ParticleManager::Get()->Play("ObstacleBreak", model->GlobalPos());
		SetActive(false);
	}
}