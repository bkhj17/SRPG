#include "framework.h"
#include "GridedTerrain.h"

GridedTerrain::GridedTerrain()
{
	material->SetShader(L"LandScape/GridedTerrain.hlsl");

	widthHeightBuffer = new FloatValueBuffer();
	widthHeightBuffer->Get()[0] = width;
	widthHeightBuffer->Get()[1] = height;
	widthHeightBuffer->Get()[2] = 10.0f;
	widthHeightBuffer->Get()[3] = 10.0f;
}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);
	__super::Render();
}
