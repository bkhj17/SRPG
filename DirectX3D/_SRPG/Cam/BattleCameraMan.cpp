#include "framework.h"

BattleCameraMan::BattleCameraMan()
{
	camera = new Camera();
	camera->SetTarget(this);
	camera->SetFollowImmadiately(true);
	
	camera->TargetOptionLoad("SRPGBattle");
	//camera->SetFollowDistance(-50.0f);
	//camera->SetRotY(-XM_PI / 6.0f);
	//camera->SetHeight(70.0f);
	//camera->TargetOptionSave("SRPGBattle");
	camera->SetRotX(-XM_PI / 9.0f);
}

BattleCameraMan::~BattleCameraMan()
{
	delete camera;
}

void BattleCameraMan::Update()
{
	UpdateWorld();
	camera->Update();
}

void BattleCameraMan::SetView()
{
	camera->SetView();
}
