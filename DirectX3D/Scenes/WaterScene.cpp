#include "framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	skybox = new SkyBox(L"Textures/LandScape/Snow_ENV.dds");
	forest = new Model("Forest");
	human = new Human;

	//reflection = new Reflection(forest);
	//refraction = new Refraction(L"Textures/LandScape/Wave.dds");

	water = new Water(L"Textures/LandScape/Wave.dds", 500, 500);
}

WaterScene::~WaterScene()
{
	delete skybox;
	delete forest;
	delete human;

	delete water;
}

void WaterScene::Update()
{
	human->Update();
	forest->UpdateWorld();

	water->Update();
}

void WaterScene::PreRender()
{
	water->SetRefraction();

	skybox->Render();
	forest->Render();
	human->Render();
	
	water->SetReflection();

	skybox->Render();
	forest->Render();
	human->Render();
}

void WaterScene::Render()
{
	skybox->Render();

	//forest->SetShader(L"Environment/Refraction.hlsl");
	forest->Render();
	water->Render();
	human->Render();
}

void WaterScene::PostRender()
{
	//reflection->PostRender();
}

void WaterScene::GUIRender()
{
	//forest->GUIRender();

	water->GUIRender();
}
