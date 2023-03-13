#pragma once

class Enemy230313 : public Transform
{
private:
	float MIN_MOVE_SPEED = 5.0f;
	float MAX_MOVE_SPEED = 15.0f;

public:
	Enemy230313();
	~Enemy230313();

	void Update();
	void UpdateWorld() override;
	void Render();
	void PostRender();

	void Spawn(Vector3 spawnPos);
	void SetTargetPos(const Vector3& pos) { targetPos = pos; }

	SphereCollider* GetCollider() { return collider; }
	Matrix GetModelWorld() { return modelTransform->GetWorld(); }

	void Hit();
private:
	void Move();
	void UpdateHeight();

private:
	Transform* modelTransform;

	Vector3 targetPos;
	float moveSpeed = 6.0f;

	SphereCollider* collider;

	int hp = 2;
	Quad* HPBar;
	IntValueBuffer* hpBuffer;
};

