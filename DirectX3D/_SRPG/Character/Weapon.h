#pragma once
class Weapon : public Transform
{
public:
	Weapon(string tag);
	~Weapon();
	
	void Update();
	void Render();

	void SetOwner(ModelAnimator* owner, int boneNum = 37) { this->owner = owner; this->boneNum = boneNum; }
	int GetPower() { return power; }
	float GetHit() { return hit; }
private:
	int power = 3;
	float hit = 100;
	pair<int, int> range;

	Model* model;

	ModelAnimator* owner = nullptr;
	int boneNum = 37;
};

