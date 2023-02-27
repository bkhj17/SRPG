#include "framework.h"
#include "Scene230227.h"
#include "TerrainEditor230227.h"

Scene230227::Scene230227()
{
	terrain = new TerrainEditor230227;
}

Scene230227::~Scene230227()
{
	delete terrain;
}

void Scene230227::Update()
{
	terrain->Update();
}

void Scene230227::PreRender()
{
}

void Scene230227::Render()
{
	terrain->Render();
}

void Scene230227::PostRender()
{
}

void Scene230227::GUIRender()
{
	terrain->RenderUI();
}
