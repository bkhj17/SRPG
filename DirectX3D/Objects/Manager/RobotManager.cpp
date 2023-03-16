#include "framework.h"

RobotManager::RobotManager()
{
	robotInstancing = new ModelAnimatorInstancing("Robot");
	robotInstancing->ReadClip("ZombieStandUp");
	robotInstancing->ReadClip("ZombieWalk");
	robotInstancing->ReadClip("ZombieRun");
	robotInstancing->ReadClip("ZombieHit");
	robotInstancing->ReadClip("ZombieDeath");

	robots.reserve(SIZE);
	for (UINT i = 0; i < SIZE; i++) {
		Transform* transform = robotInstancing->Add();
		transform->SetActive(false);
		Robot* robot = new Robot(transform, robotInstancing, i);
		robots.push_back(robot);
	}
}

RobotManager::~RobotManager()
{
	delete robotInstancing;
	for (auto robot : robots)
		delete robot;
}

void RobotManager::Update()
{
	Collision();

	time += DELTA;
	if (time > SPAWN_TIME) {
		time -= SPAWN_TIME;
		Spawn();
	}

	robotInstancing->Update();
	for (auto robot : robots)
		robot->Update();
}

void RobotManager::Render()
{
	robotInstancing->Render();
	for (auto robot : robots)
		robot->Render();
}

void RobotManager::PostRender()
{
	for (auto robot : robots)
		robot->PostRender();
}

void RobotManager::SetTarget(Transform* target)
{
	this->target = target;
	for (auto robot : robots)
		robot->SetTarget(target);
}

bool RobotManager::IsCollision(Ray ray, Vector3& hitPoint)
{
	Contact contact;
	float minDist = FLT_MAX;

	for (auto robot : robots) {
		if (robot->GetCollider()->IsRayCollision(ray, &contact)) {
			if (minDist > contact.distance) {
				minDist = contact.distance;
				hitPoint = contact.hitPoint;
			}
		}
	}

	return minDist < FLT_MAX;
}

void RobotManager::Collision()
{
	for (auto robot : robots) {
		if (KunaiManager::Get()->IsCollision(robot->GetCollider())) {
			robot->Hit();
			return;
		}
	}
}

void RobotManager::Spawn()
{
	Vector3 dir;
	dir.x = Random(-1.0f, 1.0f);
	dir.z = Random(-1.0f, 1.0f);

	Vector3 randomPos = target->Pos() + dir.GetNormalized() * 1500.0f;
	for (auto robot : robots) {
		if (!robot->GetTransform()->Active()) {
			robot->Spawn(randomPos);
			break;
		}
	}


}
