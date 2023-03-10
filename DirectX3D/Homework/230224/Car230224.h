#pragma once
class Terrain230224;
class Car230224 : public GameObject
{
public:
	Car230224();
	~Car230224();

	void Update();
	void UpdateWorld() override;

	void Render();
	void GUIRender();

	void SetTerrain(Terrain230224* terrain) { this->terrain = terrain; }
private:
	Cube* body;
	Terrain230224* terrain = nullptr;

	class Cylinder230224* wheel[4];

	float moveSpeed = 30.0f;
};

