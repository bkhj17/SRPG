#pragma once

class Bullet230313 : public BoxCollider
{
public:
	Bullet230313();
	~Bullet230313();

	void Update();
	void Render();

	void Spawn(Vector3 pos, Vector3 rot);
private:
	Cube* cube;

	float lifeTime = 20.0f;
};

