#include "framework.h"
#include "Scene230223.h"
#include "Sphere230223.h"
#include "Cube230223.h"

Scene230223::Scene230223()
{
	sphere = new Sphere230223;
	sphere->Pos().x = 10.0f;

	cube = new Cube230223;
}

Scene230223::~Scene230223()
{
	delete sphere;
	delete cube;
}

void Scene230223::Update()
{
	sphere->Rot().y += DELTA;
	if (sphere->Rot().y > 360.0f) 
		sphere->Rot().y -= 360.0f;

	sphere->Update();


	cube->Update();
}

void Scene230223::PreRender()
{
}

void Scene230223::Render()
{
	sphere->Render();

	cube->Render();
}

void Scene230223::PostRender()
{
}

void Scene230223::GUIRender()
{
	sphere->GUIRender();

	cube->GUIRender();
}