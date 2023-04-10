#pragma once

class Weapon : public Transform
{
public:
	enum Type {
		SWORD, SPEAR, AXE, BOW
	};

	struct WeaponData {
		string name;
		Type type = Weapon::SWORD;

		int power;
		float hit;
		pair<int, int> range;

		string modelName;
	};
public:
	Weapon() = default;
	~Weapon();
	
	void Update();

	void SetOwner(Transform* owner) { this->owner = owner; }
	bool HasOwner() { return owner; }

	int GetPower() { return data ? data->power : 0; }
	float GetHit() { return data ? data->hit : 0; }
	pair<int, int> GetRange() { return data ? data->range : make_pair(1, 1); }
	Type GetType() { return data ? data->type : SWORD; }
	string GetTypeString();

	void SetData(WeaponData* data) { this->data = data; }

	void SetModelTransform(string tag, Transform* transform);
private:
	WeaponData* data = nullptr;
	Transform* modelTransform = nullptr;

	Transform* owner = nullptr;
};