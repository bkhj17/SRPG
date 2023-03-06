#pragma once
class Steve : public SphereCollider
{
private:
	const float GRAVITY = 98.0f;

public:
	Steve();
	~Steve();

	void Update();
	void Render();
	void PostRender();

	void GUIRender();

private:
	void Control();
	void Jump();

private:
	float moveSpeed = 10.0f;
	float rotSpeed = 15.0f;
	float jumpPower = 20.0f;

	float velocity = 0.0f;

	bool isJump = false;

	Vector3 prevMousePos;
	Quad* crossHair;
};

