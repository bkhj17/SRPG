#include "framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	modelAnimator = new ModelAnimator("Robot");
	modelAnimator->ReadClip("Dance");
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete modelAnimator;
}

void ModelAnimationScene::Update()
{
	modelAnimator->Update();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	modelAnimator->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	modelAnimator->GUIRender();
}
