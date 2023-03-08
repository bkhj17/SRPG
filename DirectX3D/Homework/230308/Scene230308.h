#pragma once

class Scene230308 : public Scene
{
public:
	Scene230308();
	~Scene230308();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	bool play = false;

	class GameObject* player;
	vector<SphereCollider*> spots;

	int curTargetNum = 0;
	float spawnTime = 0.0f;
	float spawnRate = 5.0f;

	vector<Sphere*> targets;
	vector<SphereCollider*> bullets;
};

