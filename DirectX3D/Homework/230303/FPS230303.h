#pragma once
class FPS230303 : public Transform
{
public:
	FPS230303();
	~FPS230303();

	void Update();
	void UpdateWorld();

	BoxCollider* GetCollider() { return collider; }

	void StandOn(BoxCollider* collider);
private:
	BoxCollider* collider;

	float moveSpeed = 10.0f;

	float jumpValue = 0.0f;
};

