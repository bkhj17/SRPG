#include "framework.h"
#include "Bullet230313.h"

Bullet230313::Bullet230313()
{
	cube = new Cube();
	cube->SetParent(this);
}

Bullet230313::~Bullet230313()
{
	delete cube;
}

void Bullet230313::Update()
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

void Bullet230313::Render()
{
	if (!Active())
		return;

	__super::Render();
	cube->Render();
}

void Bullet230313::Spawn(Vector3 pos, Vector3 rot)
{
	Pos() = pos;
	Rot() = rot;

	
	SetActive(true);
	lifeTime = 20.0f;
}
