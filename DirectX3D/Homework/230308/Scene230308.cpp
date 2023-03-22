#include "framework.h"
#include "Scene230308.h"
#include "Player230308.h"
#include "Enemy230308.h"
#include "Bullet230308.h"

Scene230308::Scene230308()
{
	player = new Player230308;
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
	targets.resize(10);
	for (auto& target : targets) {
		target = new Enemy230308;
		target->SetActive(false);
	}

	bullets.resize(100);
	for (auto& bullet : bullets) {
		bullet = new Bullet230308;
		bullet->SetActive(false);
	}

	gameOver = new Quad(L"Textures/UI/GameOver.png");
	gameOver->Pos() = { CENTER_X, CENTER_Y, 0.0f };
	gameOver->UpdateWorld();
	gameOver->SetActive(false);
}

Scene230308::~Scene230308()
{
	delete player;

	for (auto spot : spots)
		delete spot;

	for (auto target : targets)
		delete target;


	for (auto bullet : bullets)
		delete bullet;

	delete gameOver;
}

void Scene230308::Update()
{
	player->Update();

	for (auto spot : spots) {
		Observer::Get()->ExcuteParamEvent("GetHeight", spot);
		spot->Pos().y += 1.0f;
		spot->UpdateWorld();
	}


	if (KEY_DOWN('K')) {
		play = !play;
		if (play) {
			CAM->SetTarget(player);
			CAM->TargetOptionLoad("Mine");
		}
	}

	if (KEY_DOWN(VK_LBUTTON)) {
		for (auto bullet : bullets) {
			if (bullet->Active())
				continue;

			bullet->Spawn(CAM->GlobalPos() + CAM->Forward() * 5.0f, CAM->Rot());
			break;
		}
	}

	for (auto bullet : bullets) {
		bullet->Update();
	}


	if (!play)
		return;

	spawnTime -= DELTA;
	if (curTargetNum < targets.size() && spawnTime <= 0.0f) {
		Vector3 spawnPos = spots[Random(0, (int)spots.size())]->GlobalPos();
		for (auto target : targets) {
			if (target->Active())
				continue;
			target->Spawn(spawnPos);
			curTargetNum++;
			spawnTime = spawnRate;
			break;
		}
	}

	for (auto target : targets) {
		if (!target->Active())
			continue;

		target->SetTargetPos(player->GlobalPos());
		target->Update();

		for (auto bullet : bullets) {
			if (!bullet->Active())
				continue;
			if (bullet->IsCollision(target->GetCollider())) {
				target->SetActive(false);
				bullet->SetActive(false);
			}
		}

		if (target->GetCollider()->IsCollision(player)) {
			play = false;
			gameOver->SetActive(true);
		}
	}
}

void Scene230308::PreRender()
{
	player->PostRender();
	gameOver->Render();
}

void Scene230308::Render()
{
	player->Render();

	for (auto spot : spots)
		spot->Render();

	for (auto target : targets) {
		if(target->Active())
			target->Render();
	}

	for (auto bullet : bullets) {
		bullet->Render();
	}
}

void Scene230308::PostRender()
{
	player->PostRender();
	gameOver->Render();
}

void Scene230308::GUIRender()
{
	player->GUIRender();
	for (auto spot : spots)
		spot->GUIRender();
}
