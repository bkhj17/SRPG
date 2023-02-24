#include "framework.h"
#include "SphereScene.h"

SphereScene::SphereScene()
{
	sun = new Sphere;
	sun->SetTag("sun");
	sun->GetMaterial()->SetDiffuseMap(L"Textures/Planets/Sun.jpg");

	earth = new Sphere;
	earth->SetTag("earth");
	earth->SetParent(sun);
	earth->GetMaterial()->SetDiffuseMap(L"Textures/Earth/Earth_DM.jpg");
	earth->GetMaterial()->SetNormalMap(L"Textures/Earth/Earth_NM.png");
}

SphereScene::~SphereScene()
{
	delete sun;
	delete earth;
}

void SphereScene::Update()
{
	sun->Rot().y += DELTA;
	sun->UpdateWorld();

	earth->UpdateWorld();
}

void SphereScene::PreRender()
{
}

void SphereScene::Render()
{
	sun->Render();
	earth->Render();
}

void SphereScene::PostRender()
{
}

void SphereScene::GUIRender()
{
	sun->RenderUI();
	earth->RenderUI();
}
