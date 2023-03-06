#pragma once
class Scene230306 : public Scene
{
public:
	Scene230306();
	~Scene230306();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	Steve* steve;
};

