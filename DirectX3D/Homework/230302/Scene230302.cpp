#include "framework.h"
#include "Scene230302.h"
#include "TerrainEditor230302.h"
Scene230302::Scene230302()
{
	terrainEditor = new TerrainEditor230302;

}

Scene230302::~Scene230302()
{
	delete terrainEditor;
}

void Scene230302::Update()
{
	terrainEditor->Update();
}

void Scene230302::PreRender()
{
}

void Scene230302::Render()
{
	terrainEditor->Render();
}

void Scene230302::PostRender()
{
}

void Scene230302::GUIRender()
{
	terrainEditor->GUIRender();
}
