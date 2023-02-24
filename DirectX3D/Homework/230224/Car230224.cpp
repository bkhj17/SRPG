#include "framework.h"
#include "Car230224.h"
#include "Terrain230224.h"
#include "Cylinder230224.h"
Car230224::Car230224()
{
	body = new Cube(Vector3(1.0f, 1.0f, 3.0f));
	body->SetTag("Body");
	body->SetParent(this);
	body->Pos().y = 1.0f;

	for (int i = 0; i < 4; i++) {
		wheel[i] = new Cylinder230224(1.0f);
		wheel[i]->SetTag("wheel" + to_string(i));
		wheel[i]->SetParent(body);
		wheel[i]->Pos().x = i % 2 == 0 ? -1.0f : 1.0f;
		wheel[i]->Pos().y = -1.0f;
		wheel[i]->Pos().z = i > 1 ? -1.5f : 1.5f;
		wheel[i]->Rot().z = XM_PIDIV2;
	}
}

Car230224::~Car230224()
{
	delete body;
	for (int i = 0; i < 4; i++)
		delete wheel[i];
}

void Car230224::Update()
{
	if (KEY_PRESS(VK_RIGHT))
		wheel[0]->Rot().y += DELTA;
	if (KEY_PRESS(VK_LEFT))
		wheel[0]->Rot().y -= DELTA;
	wheel[1]->Rot().y = wheel[0]->Rot().y;

	if (KEY_PRESS(VK_UP)) {
		float addictive = (wheel[0]->Rot().y) * DELTA;
		body->Rot().y += addictive * 2.0f;
		wheel[0]->Rot().y -= addictive;

		Pos() += body->Forward() * DELTA * moveSpeed;
	}
	if (KEY_PRESS(VK_DOWN)) {
		float addictive = (wheel[0]->Rot().y) * DELTA;
		body->Rot().y += addictive * 2.0f;
		wheel[0]->Rot().y -= addictive;

		Pos() += body->Back() * DELTA * moveSpeed;
	}

	if(terrain)
		terrain->GetHeight(Pos(), Rot());

	UpdateWorld();
}

void Car230224::UpdateWorld()
{
	__super::UpdateWorld();

	body->UpdateWorld();
	for (int i = 0; i < 4; i++) {
		if (i < 2) {
			if (KEY_PRESS(VK_UP))
				wheel[i]->Rot().x += DELTA;
			if (KEY_PRESS(VK_DOWN))
				wheel[i]->Rot().x -= DELTA;
		}
		wheel[i]->UpdateWorld();
	}
}

void Car230224::Render()
{
	body->Render();
	for (int i = 0; i < 4; i++)
		wheel[i]->Render();
}

void Car230224::RenderUI()
{
	body->RenderUI();
	for (int i = 0; i < 4; i++)
		wheel[i]->RenderUI();
}
