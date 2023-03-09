#pragma once

class Enemy230308 : public Transform
{
private:
	float MIN_MOVE_SPEED = 5.0f;
	float MAX_MOVE_SPEED = 15.0f;

public:
	Enemy230308();
	~Enemy230308();

	void Update();
	void UpdateWorld() override;
	void Render();


	void Spawn(Vector3 spawnPos);
	void SetTargetPos(const Vector3& pos) { targetPos = pos; }

	SphereCollider* GetCollider() { return collider; }
private:
	void Move();
	void UpdateHeight();

private:
	Model* model;

	Vector3 targetPos;
	float moveSpeed = 6.0f;

	SphereCollider* collider;
};

