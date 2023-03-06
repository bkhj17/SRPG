#pragma once
class Steve : public SphereCollider
{
private:
	const float GRAVITY = 9.8f;

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
	float rotSpeed = 5.0f;
	float jumpPower = 10.0f;

	float velocity = 0.0f;


	bool isJump = false;

	Quad* crossHair;
};

