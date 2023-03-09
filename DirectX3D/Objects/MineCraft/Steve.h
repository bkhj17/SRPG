#pragma once
class Steve : public SphereCollider
{
private:
	const float GRAVITY = 98.0f;

public:
	Steve();
	~Steve();

	virtual void Update();
	void Render();
	virtual void PostRender();

	void GUIRender();

	void BlocksSaveLoad();
protected:
	void Control();
	void Jump();
protected:
	MineUI* mineUI;
private:
	float moveSpeed = 20.0f;
	float rotSpeed = 5.0f;
	float jumpPower = 20.0f;

	float velocity = 0.0f;

	bool isJump = false;

	POINT clientCenterPos = { WIN_WIDTH >> 1, WIN_HEIGHT >> 1 };
	
};

