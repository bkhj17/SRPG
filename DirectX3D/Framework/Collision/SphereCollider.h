#pragma once
class SphereCollider : public Collider
{
	SphereCollider();
	~SphereCollider() = default;
	// Collider을(를) 통해 상속됨
	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;
	virtual void MakeMesh() override;
};

