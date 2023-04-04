#include "framework.h"
#include "ModelAnimationScene.h"

ModelAnimationScene::ModelAnimationScene()
{
	modelAnimator = new ModelAnimator("Soldier");
	v = {
		{"SpearIdle", 0},
		{"SpearAttack", 0},
		{"Run", 0},
		{"Hit", 0},
		{"AxeIdle", 0},
		{"AxeAttack", 0},
		{"SwordIdle", 0},
		{"SwordAttack", 0},
		{"Death", 0}
	};

	for(auto& p : v)
 		modelAnimator->ReadClip(p.first, p.second);
	
	//modelAnimator->GetClip(2)->SetEvent(bind(&ModelAnimationScene::SetIdle, this), 0.55f);

	hand = new Transform;
	sword = new Model("Sword");
	sword->SetTag("Sword");
	sword->Load();
	sword->SetParent(hand);
}

ModelAnimationScene::~ModelAnimationScene()
{
	delete modelAnimator;

	delete hand;
	delete sword;
}

void ModelAnimationScene::Update()
{
	for (int i = 0; i < v.size()+1; i++) {
		if (KEY_DOWN('1'+i))
			modelAnimator->PlayClip(i);
	}

	modelAnimator->Update();

	hand->SetWorld(modelAnimator->GetTransformByNode(handNode));
	sword->UpdateWorld();
}

void ModelAnimationScene::PreRender()
{
}

void ModelAnimationScene::Render()
{
	modelAnimator->Render();

	sword->Render();
}

void ModelAnimationScene::PostRender()
{
}

void ModelAnimationScene::GUIRender()
{
	modelAnimator->GUIRender();

	sword->GUIRender();

	ImGui::SliderInt("HandNode", &handNode, 0, 100);
}

void ModelAnimationScene::SetIdle()
{
	modelAnimator->PlayClip(0, 1.0f, 0.1f);
}
