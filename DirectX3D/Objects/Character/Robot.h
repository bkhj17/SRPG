#pragma once
class Robot
{
public:
	Robot(Transform* transform);
	~Robot();

	void Update();
	void Render();

	SphereCollider* GetCollider() { return collider; }
	Transform* GetTransform() { return transform; }
private:
	Transform* transform;
	SphereCollider* collider;

	float speed = 300.0f;

	Vector3 velocity;
};

