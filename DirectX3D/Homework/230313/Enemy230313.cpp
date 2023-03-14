#include "framework.h"
#include "Enemy230313.h"

Enemy230313::Enemy230313()
{
	modelTransform = new Transform;
	modelTransform->SetParent(this);
	modelTransform->Scale() *= 0.05f;

	collider = new SphereCollider;
	collider->SetParent(this);
	collider->Scale() *= 3.0f;
	collider->Pos().y += 5.0f;

	HPBar = new Quad(L"Textures/UI/QuickSlot.png");
	HPBar->GetMaterial()->SetShader(L"ProgressBar230313.hlsl");

	hpBuffer = new IntValueBuffer;
}

Enemy230313::~Enemy230313()
{
	delete modelTransform;
	delete collider;
	delete HPBar;
	delete hpBuffer;
}

void Enemy230313::Update()
{
	if (!Active())
		return;

	Move();
	UpdateHeight();
	UpdateWorld();
}

void Enemy230313::UpdateWorld()
{
	if (!Active())
		return;

	__super::UpdateWorld();
	modelTransform->UpdateWorld();
	collider->UpdateWorld();

}

void Enemy230313::Render()
{
	if (!Active())
		return;

	collider->Render();
}

void Enemy230313::PostRender()
{
	if (!Active())
		return;

	Vector3 gPos = GlobalPos();
	gPos.y += 10.0f;



	HPBar->Pos() = CAM->WorldToScreen(gPos);
	HPBar->UpdateWorld();
	hpBuffer->Get()[0] = 2;
	hpBuffer->Get()[1] = hp;
	hpBuffer->SetPS(9);
	HPBar->Render();
}

void Enemy230313::Spawn(Vector3 spawnPos)
{
	Pos() = spawnPos;
	moveSpeed = Random(MIN_MOVE_SPEED, MAX_MOVE_SPEED);
	hp = 2;

	SetAction(0);
	UpdateWorld();
	SetActive(true);
}

void Enemy230313::Hit()
{
	if (!Active() || curClip < 2)
		return;

	 
	SetAction((--hp <= 0) ? 2 : 1);	
}

void Enemy230313::SetAction(int index)
{
	if (curClip == index)
		return;

	curClip = index;
	//instancing에 접근해야 한다

}

void Enemy230313::Move()
{
	if (curClip != 0)
		return;

	Vector3 direction = (targetPos - GlobalPos());
	direction.Normalize();

	{
		Vector3 a = Vector3(0, 0, -1);
		Vector3 b = direction;
		b.y = 0.0f;

		Vector3 aNorm = a.GetNormalized();
		Vector3 bNorm = b.GetNormalized();

		float yaw = atan2f(bNorm.x, bNorm.z) - atan2f(aNorm.x, aNorm.z);
		
		Vector3 rot = { 0.0f, yaw, 0.0f };

		modelTransform->Rot() = Lerp(modelTransform->Rot(), rot, DELTA * 10.0f);
	}

	Pos() += direction * moveSpeed * DELTA;
}

void Enemy230313::UpdateHeight()
{
	Observer::Get()->ExcuteParamEvent("GetHeightNormal", this);
}
