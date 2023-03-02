#pragma once

class Scene230302 : public Scene
{
public:
	Scene230302();
	~Scene230302();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	class TerrainEditor230302* terrainEditor;
};

