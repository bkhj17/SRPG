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

		XMVECTOR aNorm = XMVector3Normalize(a);
		XMVECTOR bNorm = XMVector3Normalize(b);

		float yaw = atan2f(XMVectorGetX(bNorm), XMVectorGetZ(bNorm)) -
			atan2f(XMVectorGetX(aNorm), XMVectorGetZ(aNorm));
		float pitch = asinf(XMVectorGetY(bNorm)) - asinf(XMVectorGetY(aNorm));

		XMVECTOR aCrossB = XMVector3Cross(a, b);
		float roll = atan2f(XMVectorGetX(aCrossB), XMVectorGetY(aCrossB));

		model->Rot() = { 0.0f, yaw, 0.0f };
	}

	Pos() += direction * moveSpeed * DELTA;
}

void Enemy230308::UpdateHeight()
{
	Observer::Get()->ExcuteParamEvent("GetHeightNormal", this);
}
