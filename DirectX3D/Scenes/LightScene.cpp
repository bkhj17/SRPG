#include "framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	sphere = new Sphere(10);
}

LightScene::~LightScene()
{
	delete sphere;
}

void LightScene::Update()
{
	sphere->UpdateWorld();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	sphere->Render();
}

void LightScene::PostRender()
{
}

void LightScene::GUIRender()
{
	sphere->RenderUI();
}
