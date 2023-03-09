#include "framework.h"
#include "Bullet230308.h"

Bullet230308::Bullet230308()
{
	cube = new Cube();
	cube->SetParent(this);
}

Bullet230308::~Bullet230308()
{
	delete cube;
}

void Bullet230308::Update()
{
	if (!Active())
		return;

	Pos() += Forward() * DELTA * 50.0f;

	lifeTime -= DELTA;
	if(lifeTime <= 0.0f)
		SetActive(false);

	UpdateWorld();
	cube->UpdateWorld();
}

void Bullet230308::Render()
{
	if (!Active())
		return;

	__super::Render();
	cube->Render();
}

void Bullet230308::Spawn(Vector3 pos, Vector3 rot)
{
	Pos() = pos;
	Rot() = rot;

	
	SetActive(true);
	lifeTime = 20.0f;
}
