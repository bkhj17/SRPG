#include "framework.h"
#include "Scene230313.h"
#include "Player230313.h"
#include "Enemy230313.h"
#include "Bullet230313.h"
#include "Instancing230313.h"

Scene230313::Scene230313()
{
	player = new Player230313;
	player->Pos().x = 125.0f;
	player->Pos().z = 45.0f;
	player->Scale() *= 5.0f;

	spots.resize(4);
	int index = 0;
	for (auto& spot : spots) {
		spot = new SphereCollider;
		spot->Pos() = { 50.0f * (index+1), 0.0f, 200.0f };
		spot->SetTag("spot" + to_string(index++));
	}

	inst = new Instancing230313("Robot");
	inst->ReadClip("Walking", 0);
	for (int i = 0; i < 10; i++) {
		auto target = inst->Add(new Enemy230313);
		target->SetActive(false);
		
	}

	bullets.resize(100);
	for (auto& bullet : bullets) {
		bullet = new Bullet230313;
		bullet->SetActive(false);
	}

	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = { CENTER_X, CENTER_Y, 0.0f };
	crossHair->UpdateWorld();

	gameOver = new Quad(L"Textures/UI/GameOver.png");
	gameOver->Pos() = { CENTER_X, CENTER_Y, 0.0f };
	gameOver->UpdateWorld();
	gameOver->SetActive(false);

	Observer::Get()->AddEvent("Fire", bind(&Scene230313::Fire, this));
}

Scene230313::~Scene230313()
{
	delete player;

	for (auto spot : spots)
		delete spot;

	for (auto bullet : bullets)
		delete bullet;

	delete crossHair;
	delete gameOver;

	delete inst;
}

void Scene230313::Update()
{
	player->Update();

	for (auto spot : spots) {
		Observer::Get()->ExcuteParamEvent("GetHeight", spot);
		spot->Pos().y += 1.0f;
		spot->UpdateWorld();
	}


	if (KEY_DOWN('K'))
		play = !play;

	for (auto bullet : bullets) {
		bullet->Update();
	}

	if (!play)
		return;

	auto& targets = inst->GetTransforms();

	spawnTime -= DELTA;
	if (spawnTime <= 0.0f) {
		Vector3 spawnPos = spots[Random(0, spots.size())]->GlobalPos();
		if(inst->Spawn(spawnPos))
			spawnTime = spawnRate;
	}
	for (auto target : targets) {
		if (!target->Active())
			continue;

		auto enemy = (Enemy230313*)target;
		enemy->SetTargetPos(player->GetShotPos());
		enemy->Update();

		for (auto bullet : bullets) {
			if (!bullet->Active())
				continue;
			if (bullet->IsCollision(enemy->GetCollider())) {
				bullet->SetActive(false);
				enemy->Hit();
			}
		}

		if (enemy->GetCollider()->IsCollision(player->GetCollider())) {
			play = false;
			gameOver->SetActive(true);
		}
	}
	inst->Update();
}

void Scene230313::PreRender()
{
}

void Scene230313::Render()
{
	player->Render();

	for (auto spot : spots)
		spot->Render();

	inst->Render();

	for (auto bullet : bullets) {
		bullet->Render();
	}
}

void Scene230313::PostRender()
{
	inst->PostRender();
	crossHair->Render();
	gameOver->Render();
}

void Scene230313::GUIRender()
{
	player->GUIRender();
	for (auto spot : spots)
		spot->GUIRender();
}

Vector3 Scene230313::GetShotDist()
{
	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));

	Contact contact;
	Contact mn;
	mn.distance = FLT_MAX;

	auto& targets = inst->GetTransforms();

	for (auto target : targets) {
		if (!target->Active())
			continue;

		auto enemy = (Enemy230313*)target;
		if (enemy->GetCollider()->IsRayCollision(ray, &contact)) {
			if (contact.distance < mn.distance)
				mn = contact;
		}
	}
	if (mn.distance < FLT_MAX)
		return mn.hitPoint;

	Vector3 pos;
	Observer::Get()->ExcuteParamEvent("GetPickingPos", &pos);
	return pos;
}

void Scene230313::Fire()
{
	for (auto bullet : bullets) {
		if (bullet->Active())
			continue;

		Vector3 shotPos = player->GlobalPos();
		shotPos.y += 5.0f;
		Vector3 shotDist = GetShotDist();
		Vector3 vec = shotDist - shotPos;
		vec.Normalize();

		Vector3 rot = { -asinf(vec.y), atanf(vec.x / vec.z), 0.0f };

		bullet->Spawn(shotPos, rot);
		break;
	}
}
