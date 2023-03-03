#include "framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	sphere = new Sphere(10);
	sphere->GetMaterial()->GetName() = "FieldStone";

}

LightScene::~LightScene()
{
	delete sphere;
}

void LightScene::Update()
{
	if (KEY_PRESS(VK_F3))
		sphere->GetMaterial()->Save("TextData/Materials/FieldStone.material");

	if (KEY_PRESS(VK_F4))
		sphere->GetMaterial()->Load("TextData/Materials/FieldStone.material");

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
