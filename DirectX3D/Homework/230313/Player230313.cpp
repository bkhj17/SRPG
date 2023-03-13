#include "framework.h"
#include "Player230313.h"

Player230313::Player230313()
{

	model = new ModelAnimator("Naruto");
	model->SetTag("Naruto");
	model->SetParent(this);
	model->Scale() *= 0.01f;
	model->Rot().y = XM_PI;

	model->ReadClip("Idle");
	model->ReadClip("Run");
	model->ReadClip("Firing");
	model->GetClip(2)->SetEvent(bind(&Player230313::SetAction, this, 0), 1.0f);
	model->GetClip(2)->SetEvent(bind(&Player230313::Fire, this), 0.5f);
	
	model->PlayClip(curClip);

	rightHand = new Transform();
	rightHand->UpdateWorld();

	gun = new Model("Rifle");
	gun->SetTag("Rifle");
	gun->SetParent(rightHand);
	gun->Scale() *= 100.0f;

	//CAM->SetTarget(this);

	collider = new SphereCollider;
	collider->SetParent(this);
	collider->Pos().y += 1.0f;
}

Player230313::~Player230313()
{
	delete gun;
	delete rightHand;
	delete model;
	delete collider;
}

void Player230313::Update()
{
	Observer::Get()->ExcuteParamEvent("GetHeight", this);

	if (KEY_DOWN(VK_LBUTTON)) {
		if (curClip != 2) {
			SetAction(2);
		}
	}

	if (curClip != 2) {
		int clip = 0;

		if (!KEY_PRESS(VK_RBUTTON)) {
			if (KEY_PRESS('W')) {
				Pos() += Forward() * DELTA * 30.0f;
				clip = 1;
			}
			if (KEY_PRESS('D')) {
				Rot().y += DELTA;
			}
			if (KEY_PRESS('A')) {
				Rot().y -= DELTA;
			}
		}
		SetAction(clip);
	}

	UpdateWorld();
	model->Update();
	collider->UpdateWorld();

	Matrix mat = (Matrix&)model->GetTransformByNode(38);
	mat = mat * model->GetWorld();
	rightHand->SetWorld(mat);
	gun->UpdateWorld();
}

void Player230313::Render()
{
	model->Render();
	gun->Render();
	collider->Render();
}

void Player230313::PostRender()
{
}

void Player230313::GUIRender()
{
	model->GUIRender();
	gun->GUIRender();
}

Vector3 Player230313::GetShotPos()
{
	return rightHand->GlobalPos();
}

void Player230313::SetAction(int clip)
{
	if (clip == curClip && curClip != 2)
		return;

	curClip = clip;
	model->PlayClip(clip);
}

void Player230313::Fire()
{
	Observer::Get()->ExcuteEvent("Fire");
}

