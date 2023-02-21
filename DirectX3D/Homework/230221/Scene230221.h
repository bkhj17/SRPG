#pragma once
class Planet230221;
class Scene230221 : public Scene
{
public:
	Scene230221();
	~Scene230221();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;
private:
	Planet230221* sun;
	Planet230221* mercury;
	Planet230221* venus;
	Planet230221* earth;
	Planet230221* moon;
	Planet230221* Mars;
	Planet230221* jupiter;

	class Grid230221* grid;
};

