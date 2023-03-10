#pragma once

class GridedTerrain : public Terrain
{
public:
	GridedTerrain();
	~GridedTerrain();

	void Render();

private:
	FloatValueBuffer* widthHeightBuffer;
};

