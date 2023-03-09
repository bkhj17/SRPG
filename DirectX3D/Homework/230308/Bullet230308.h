#pragma once

class Bullet230308 : public BoxCollider
{
public:
	Bullet230308();
	~Bullet230308();

	void Update();
	void Render();

	void Spawn(Vector3 pos, Vector3 rot);
private:
	Cube* cube;

	float lifeTime = 20.0f;
};

