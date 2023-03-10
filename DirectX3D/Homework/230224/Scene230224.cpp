#include "framework.h"
#include "Scene230224.h"
#include "Terrain230224.h"
#include "Car230224.h"
Scene230224::Scene230224()
{
	terrain = new Terrain230224;
	car = new Car230224;
	car->SetTerrain(terrain);
}

Scene230224::~Scene230224()
{
	delete terrain;
	delete car;
}

void Scene230224::Update()
{
	car->Update();
}

void Scene230224::PreRender()
{
}

void Scene230224::Render()
{
	terrain->Render();
	car->Render();
}

void Scene230224::PostRender()
{
}

void Scene230224::GUIRender()
{
	terrain->GUIRender();
	car->GUIRender();
}
