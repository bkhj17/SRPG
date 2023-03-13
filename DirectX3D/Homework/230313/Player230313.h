#pragma once

class Player230313 : public Transform
{
public:
	Player230313();
	~Player230313();

	virtual void Update();
	void Render();
	void PostRender();
	void GUIRender() override;

	SphereCollider* GetCollider() { return collider; }

	Vector3 GetShotPos();

private:
	void SetAction(int clip);
	void Fire();
private:
	SphereCollider* collider;
	ModelAnimator* model;
	Transform* rightHand;

	Model* gun;

	int curClip = 0;
};

