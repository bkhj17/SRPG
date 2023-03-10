#include "framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	modelAnimator = new ModelAnimator("Human");
	//modelAnimator->ReadClip("Dance");
	//modelAnimator->ReadClip("idle");
	modelAnimator->ReadClip("idle", 1);
	//modelAnimator->ReadClip("jump");
	modelAnimator->ReadClip("jump", 1);
	//modelAnimator->ReadClip("run");
	modelAnimator->ReadClip("run", 1);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete modelAnimator;
}

void ModelAnimationScene::Update()
{

	for (int i = 0; i < 3; i++) {
		if (KEY_DOWN('1'+i))
			modelAnimator->PlayClip(i);
	}

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
