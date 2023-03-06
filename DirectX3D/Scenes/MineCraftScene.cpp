#include "framework.h"
#include "MineCraftScene.h"

MineCraftScene::MineCraftScene()
{
	BlockManager::Get()->CreateBlocks(10, 10);
	steve = new Steve;
	steve->Pos() = { 5,5,5 };
	steve->UpdateWorld();
}

MineCraftScene::~MineCraftScene()
{
	BlockManager::Delete();
	delete steve;
}

void MineCraftScene::Update()
{
	BlockManager::Get()->Update();
	steve->Update();
}

void MineCraftScene::PreRender()
{
}

void MineCraftScene::Render()
{
	BlockManager::Get()->Render();
	steve->Render();
}

void MineCraftScene::PostRender()
{
	steve->PostRender();
}

void MineCraftScene::GUIRender()
{
}
