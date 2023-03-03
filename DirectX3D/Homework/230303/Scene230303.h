#pragma once
class Cube230303;
class Scene230303 : public Scene
{
public:
	Scene230303();
	~Scene230303();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	vector<Cube230303*> cubes;
	Cube230303* check = nullptr;

	class FPS230303* fps;
};

