#pragma once

class Scene230313 : public Scene
{
public:
	Scene230313();
	~Scene230313();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	Vector3 GetShotDist();
	void Fire();
private:
	bool play = false;

	class Player230313* player;
	vector<SphereCollider*> spots;

	float spawnTime = 0.0f;
	float spawnRate = 5.0f;

	vector<class Bullet230313*> bullets;

	Quad* crossHair;
	Quad* gameOver;

	class Instancing230313* inst;
};

