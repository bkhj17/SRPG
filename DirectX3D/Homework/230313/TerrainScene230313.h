#pragma once

class TerrainScene230313 : public Scene
{
public:
	TerrainScene230313();
	~TerrainScene230313();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void GetHeightFromTerrain(void* transformPtr, bool updateNormal = true);
	void ComputePicking(void* pick);
private:
	class Terrain230313* terrain;
};

