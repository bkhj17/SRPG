#include "framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	modelAnimator = new ModelAnimator("Soldier");
	v = {
		{"SpearIdle", 0},
		{"SpearAttack", 0},
		{"Run", 0},
		{"AxeIdle", 0},
		{"AxeAttack", 0},
		{"SwordIdle", 0},
		{"SwordAttack", 0},
		{"Death", 0}
	};

	for(auto& p : v)
 		modelAnimator->ReadClip(p.first, p.second);
	

	//modelAnimator->GetClip(2)->SetEvent(bind(&ModelAnimationScene::SetIdle, this), 0.55f);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete modelAnimator;
}

void ModelAnimationScene::Update()
{
	for (int i = 0; i < v.size(); i++) {
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
