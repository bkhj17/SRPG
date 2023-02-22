#pragma once

class TerrainScene230222 : public Scene
{
public:
	TerrainScene230222();
	~TerrainScene230222();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	class Terrain230222* terrain;
};


