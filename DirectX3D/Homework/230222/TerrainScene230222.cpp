#include "framework.h"
#include "TerrainScene230222.h"
#include "Terrain230222.h"

TerrainScene230222::TerrainScene230222()
{
	terrain = new Terrain230222;
}

TerrainScene230222::~TerrainScene230222()
{
	delete terrain;
}

void TerrainScene230222::Update()
{
}

void TerrainScene230222::PreRender()
{
}

void TerrainScene230222::Render()
{
	terrain->Render();
}

void TerrainScene230222::PostRender()
{
}

void TerrainScene230222::GUIRender()
{
	terrain->RenderUI();
}
