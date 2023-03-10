#include "framework.h"
#include "Enemy230308.h"

Enemy230308::Enemy230308()
{
	model = new Model("Robot");
	model->SetParent(this);
	model->Scale() *= 0.05f;

	collider = new SphereCollider;
	collider->SetParent(this);
	collider->Scale() *= 3.0f;
	collider->Pos().y += 5.0f;
}

Enemy230308::~Enemy230308()
{
	delete model;
	delete collider;
}

void Enemy230308::Update()
{
	if (!Active())
		return;

	Move();
	UpdateHeight();
	UpdateWorld();
}

void Enemy230308::UpdateWorld()
{
	__super::UpdateWorld();
	model->UpdateWorld();
	collider->UpdateWorld();
}

void Enemy230308::Render()
{
	if (!Active())
		return;

	model->Render();
	collider->Render();
}

void Enemy230308::Spawn(Vector3 spawnPos)
{
	Pos() = spawnPos;
	moveSpeed = Random(MIN_MOVE_SPEED, MAX_MOVE_SPEED);
	UpdateWorld();
	SetActive(true);
}

void Enemy230308::Move()
{
	//타겟을 바라보는건 model rot을 
	//지형 normal 받는건 본체 rot을

	// 시작 위치에서 목표 위치를 바라보는 방향을 계산합니다.
	Vector3 direction = (targetPos - GlobalPos());
	direction.Normalize();

	{
		Vector3 a = Vector3(0, 0, -1);
		Vector3 b = direction;
		b.y = 0.0f;

		Vector3 aNorm = a.GetNormalized();
		Vector3 bNorm = b.GetNormalized();

		float yaw = atan2f(bNorm.x, bNorm.z) - atan2f(aNorm.x, aNorm.z);
		
		/*
		float pitch = asinf(bNorm.y) - asinf(aNorm.y);
		
		Vector3 aCrossB = XMVector3Cross(a, b);
		float roll = atan2f(aCrossB.x, aCrossB.y);
		*/
		Vector3 rot = { 0.0f, yaw, 0.0f };

		model->Rot() = Lerp(model->Rot(), rot, DELTA * 10.0f);
	}

	Pos() += direction * moveSpeed * DELTA;
}

void Enemy230308::UpdateHeight()
{
	Observer::Get()->ExcuteParamEvent("GetHeightNormal", this);
}
