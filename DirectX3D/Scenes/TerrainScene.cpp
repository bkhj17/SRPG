#include "framework.h"
#include "TerrainScene.h"

TerrainScene::TerrainScene()
{
	terrain = new Terrain;
	//terrain->Pos() = { -128, 0, -128 };
	//terrain->SetPivot({ 128, 0, 128 });
	terrain->UpdateWorld();

	cylinder = new Cylinder;
	cylinder->Rot().z = XM_PIDIV2;
}

TerrainScene::~TerrainScene()
{
	delete terrain;
	delete cylinder;
}

void TerrainScene::Update()
{
	if (KEY_PRESS(VK_UP)) {
		cylinder->Pos().z += DELTA * 30.0f;
	}
	if (KEY_PRESS(VK_DOWN)) {
		cylinder->Pos().z -= DELTA * 30.0f;
	}
	if (KEY_PRESS(VK_RIGHT))
		cylinder->Pos().x += DELTA * 30.0f; 
	if(KEY_PRESS(VK_LEFT))
		cylinder->Pos().x -= DELTA * 30.0f;
	//terrain->Rot().y += DELTA;
	Vector3 normal;
	cylinder->Pos().y = terrain->GetHeight(cylinder->Pos(), &normal);
	cylinder->Rot() = GetRotFromNormal(normal);
	cylinder->UpdateWorld();
}

void TerrainScene::PreRender()
{
}

void TerrainScene::Render()
{
	terrain->Render();
	cylinder->Render();
}

void TerrainScene::PostRender()
{
}

void TerrainScene::GUIRender()
{
	terrain->GUIRender();
	cylinder->GUIRender();
}
