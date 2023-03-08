#include "framework.h"
#include "Scene230308.h"

Scene230308::Scene230308()
{
	player = new SphereCollider;
	//CAM->SetTarget(player);

	spots.resize(4);
	int index = 0;
	for (auto& spot : spots) {
		spot = new SphereCollider;
		spot->Pos() = { 25.0f * index, 0.0f, 100.0f };
		spot->UpdateWorld();
		spot->SetTag("spot" + to_string(index++));
	}
	targets.resize(10);
	for (auto& target : targets) {
		target = new Sphere;
		target->GetMaterial()->SetDiffuseMap(L"Textures/Block/Gold.png");
		target->SetActive(false);
	}
}

Scene230308::~Scene230308()
{
	delete player;

	for (auto spot : spots)
		delete spot;

	for (auto target : targets)
		delete target;
}

void Scene230308::Update()
{
	if (KEY_DOWN('K'))
		play = !play;
	if (!play)
		return;

	player->UpdateWorld();
	for (auto spot : spots)
		spot->UpdateWorld();

	spawnTime -= DELTA;
	if (curTargetNum < targets.size() && spawnTime <= 0.0f) {
		Vector3 spawnPos = spots[Random(0, spots.size())]->GlobalPos();
		for (auto target : targets) {
			if (target->Active())
				continue;

			target->Pos() = spawnPos;
			target->UpdateWorld();
			target->SetActive(true);
			curTargetNum++;
			spawnTime = spawnRate;
		}
	}

	for (auto target : targets) {
		if (!target->Active())
			continue;

		Vector3 vec = player->GlobalPos() - target->GlobalPos();
		Vector3 curForward(0,0,1);
		vec.y = 0;
		vec.Normalize();


		Vector3 rotAxis = Cross(vec, curForward);
		float rotAngle = Dot(curForward, vec);

		float radian = acosf(rotAngle);
		float degree = XMConvertToDegrees(radian);
		
		if (rotAxis == Vector3::Zero())
			target->Rot() = rotAxis;
		else
			target->Rot() = XMQuaternionRotationAxis(rotAxis.GetNormalized(), radian);
		
		target->Pos() += target->Forward() * 30.0f * DELTA;

		target->UpdateWorld();
	}
}

void Scene230308::PreRender()
{
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
}

void Scene230308::PostRender()
{
}

void Scene230308::GUIRender()
{
	player->RenderUI();
	for (auto spot : spots)
		spot->RenderUI();

	
}
