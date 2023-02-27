#pragma once
class Scene230227 : public Scene
{
public:
	Scene230227();
	~Scene230227();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	class TerrainEditor230227* terrain;
};

