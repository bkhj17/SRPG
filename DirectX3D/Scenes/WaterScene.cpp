#include "framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	skybox = new SkyBox(L"Textures/LandScape/Snow_ENV.dds");
	forest = new Model("Forest");
	human = new Human;

	//reflection = new Reflection(forest);
	//refraction = new Refraction(L"Textures/LandScape/Wave.dds");

	water = new Water(L"Textures/LandScape/Wave.dds");
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
	water->Update();

	forest->UpdateWorld();
}

void WaterScene::PreRender()
{
	water->SetReflection();

	skybox->Render();
	human->Render();
	
	water->SetRefraction();

	skybox->Render();
	human->Render();
}

void WaterScene::Render()
{
	skybox->Render();

	//forest->SetShader(L"Environment/Refraction.hlsl");
	human->Render();
	water->Render();
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
