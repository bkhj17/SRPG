#include "framework.h"
#include "Scene230306.h"
#include "BlockManager230306.h"
#include "Inventory230306.h"

Scene230306::Scene230306()
{
	BlockManager230306::Get()->CreateBlocks(10, 10);
	steve = new Steve;
	steve->Pos() = { 5,5,5 };
	steve->UpdateWorld();

	CAM->SetTarget(steve);
	CAM->TargetOptionLoad("Mine");

	Inventory230306::Get();
}

Scene230306::~Scene230306()
{
	BlockManager230306::Delete();
	delete steve;
	Inventory230306::Delete();
}

void Scene230306::Update()
{
	BlockManager230306::Get()->Update();
	steve->Update();


	Ray ray(steve->GlobalPos(), CAM->Forward());
	Contact hit;

	Cube* cube = BlockManager230306::Get()->BlockMining(ray, &hit);
	if (KEY_DOWN(VK_RBUTTON) && cube) {
		Texture* t = cube->GetMaterial()->GetDiffuseMap();
		Inventory230306::Get()->Input(t);
		cube->SetActive(false);
	}
	if (KEY_DOWN(VK_LBUTTON) && cube) {
		auto texture = Inventory230306::Get()->Select();
		if (BlockManager230306::Get()->SetupCube(cube, texture, hit))
			Inventory230306::Get()->OutputSelect();
	}

	Inventory230306::Get()->Update();

}

void Scene230306::PreRender()
{
}

void Scene230306::Render()
{
	BlockManager230306::Get()->Render();
	steve->Render();
}

void Scene230306::PostRender()
{
	steve->PostRender();
	Inventory230306::Get()->Render();
}

void Scene230306::GUIRender()
{
	steve->GUIRender();
}
