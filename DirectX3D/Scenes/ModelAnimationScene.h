#pragma once
#include "Scene.h"
class ModelAnimationScene : public Scene
{
public:
	ModelAnimationScene();
	~ModelAnimationScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void SetIdle();

private:
	ModelAnimator* modelAnimator;
	vector<pair<string, int>> v;

	Transform* hand;
	Model* weapon;
	int handNode = 37;
};

