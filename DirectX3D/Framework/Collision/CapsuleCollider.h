#pragma once
class CapsuleCollider : public Collider
{
public:
	CapsuleCollider();
	~CapsuleCollider();
	// Collider을(를) 통해 상속됨
	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
private:
	virtual void MakeMesh() override;
};

