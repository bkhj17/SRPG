#include "framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	modelAnimator = new ModelAnimator("Naruto");
	modelAnimator->ReadClip("Idle", 0);
	modelAnimator->ReadClip("Run", 0);
	modelAnimator->ReadClip("Punching", 0);

	modelAnimator->GetClip(2)->SetEvent(bind(&ModelAnimationScene::SetIdle, this), 0.55f);
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

void ModelAnimationScene::SetIdle()
{
	modelAnimator->PlayClip(0, 1.0f, 0.1f);
}
