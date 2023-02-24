#include "framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain;
	terrain->Pos() = { -128, 0, -128 };
	terrain->SetPivot({ 128, 0, 128 });
	terrain->UpdateWorld();
}

TerrainScene::~TerrainScene()
{
	delete terrain;
}

void TerrainScene::Update()
{
	//terrain->Rot().y += DELTA;
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->RenderUI();
}
