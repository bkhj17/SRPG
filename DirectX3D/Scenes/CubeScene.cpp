#include "framework.h"
#include "CubeScene.h"

CubeScene::CubeScene()
{
	cube = new Cube();
	cube->GetMaterial()->SetDiffuseMap(L"Textures/Planets/Sun.jpg");
}

CubeScene::~CubeScene()
{
	delete cube;
}

void CubeScene::Update()
{
	if (KEY_PRESS(VK_RIGHT))
		cube->Rot().y += DELTA;
	if (KEY_PRESS(VK_LEFT))
		cube->Rot().y -= DELTA;
	cube->Update();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
	cube->Render();
}

void CubeScene::PostRender()
{
}

void CubeScene::GUIRender()
{
	cube->GUIRender();
	cube->GetMaterial()->GUIRender();
}
