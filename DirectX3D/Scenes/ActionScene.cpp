#include "framework.h"
#include "ActionScene.h"

ActionScene::ActionScene()
{
	human = new Human;
}

ActionScene::~ActionScene()
{
	delete human;
}

void ActionScene::Update()
{
	human->Update();

}

void ActionScene::PreRender()
{
}

void ActionScene::Render()
{
	human->Render();
}

void ActionScene::PostRender()
{
}

void ActionScene::GUIRender()
{
	human->GUIRender();
}
