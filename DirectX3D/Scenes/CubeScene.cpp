#include "framework.h"
#include "CubeScene.h"

CubeScene::CubeScene()
{
	cube = new Cube();
	cube2 = new Cube();
	cube2->SetTag("Cube2");
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
	cube2->Update();
}

void CubeScene::PreRender()
{
}

void CubeScene::Render()
{
	cube->Render();
	cube2->Render();
}

void CubeScene::PostRender()
{
}

void CubeScene::GUIRender()
{
	cube->RenderUI();
	cube2->RenderUI();
}
