#pragma once
class BoxCollider : public Collider
{
public:
	struct ObbDesc {
		Vector3 pos;
		Vector3 axis[3];
		Vector3 halfSize;
	};

public:
	BoxCollider(Vector3 size = Vector3(1,1,1));
	~BoxCollider() = default;

	// Collider을(를) 통해 상속됨
	virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool IsBoxCollision(BoxCollider* collider) override;
	virtual bool IsSphereCollision(SphereCollider* collider) override;
	virtual bool IsCapsuleCollision(CapsuleCollider* collider) override;

	void GetObb(OUT ObbDesc& obbDesc);
private:
	bool IsSeperateAxis(Vector3 D, Vector3 axis, ObbDesc box1, ObbDesc box2);

	virtual void MakeMesh() override;

private:
	Vector3 size;
};

