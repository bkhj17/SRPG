#include "framework.h"
#include "Car230224.h"
#include "Terrain230224.h"

Car230224::Car230224()
{
	body = new Cube;
	body->SetParent(this);
	body->Pos().y = 1.0f;
	body->Scale().z = 3.0f;
}

Car230224::~Car230224()
{
	delete body;
}

void Car230224::Update()
{
	if (KEY_PRESS(VK_UP))
		Pos() += body->Forward() * DELTA * moveSpeed;
	if (KEY_PRESS(VK_DOWN))
		Pos() += body->Back() * DELTA * moveSpeed;
	if (KEY_PRESS(VK_RIGHT))
		body->Rot().y += DELTA;
	if (KEY_PRESS(VK_LEFT))
		body->Rot().y -= DELTA;


	if(terrain)
		terrain->GetHeight(Pos(), Rot());

	UpdateWorld();
}

void Car230224::UpdateWorld()
{
	__super::UpdateWorld();
	body->UpdateWorld();
}

void Car230224::Render()
{
	body->Render();
}
