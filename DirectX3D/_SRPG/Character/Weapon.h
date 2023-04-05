#pragma once

struct WeaponData {
	string name;

	int power;
	float hit;
	pair<int, int> range;

	string modelName;
};

class Weapon : public Transform
{
public:
	enum Type {
		SWORD, SPEAR, AXE, BOW
	};

public:
	Weapon() = default;
	~Weapon();
	
	void Update();

	void SetOwner(ModelAnimator* owner, int boneNum = 37) { this->owner = owner; this->boneNum = boneNum; }
	bool HasOwner() { return owner; }

	int GetPower() { return data ? data->power : 0; }
	float GetHit() { return data ? data->hit : 0; }

	void SetData(WeaponData* data) { this->data = data; }
	void SetModelTransform(string tag, Transform* transform);
private:
	WeaponData* data = nullptr;
	Transform* modelTransform = nullptr;

	ModelAnimator* owner = nullptr;
	int boneNum = 37;
};