#include "framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
	models = new ModelAnimatorInstancing("Naruto");
	models->ReadClip("Idle");
	models->ReadClip("Punching");

	for (float z = 0; z < SIZE; z++) {
		for (float x = 0; x < SIZE; x++) {
			Transform* transform = models->Add();
			transform->Pos() = { x, 0.0f, z };
			transform->Scale() *= 0.01f;
		}
	}

}

InstancingScene::~InstancingScene()
{
	delete models;
}

void InstancingScene::Update()
{
	for (int i = 0; i < 3; i++) {
		if (KEY_DOWN('1' + i)) {
			for(int j = 0; j < MAX_INSTANCE; j++)
				models->PlayClip(j, i);
		}
	}

	models->Update();
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
	models->Render();
}

void InstancingScene::PostRender()
{
}

void InstancingScene::GUIRender()
{
	ImGui::SliderInt("ID", &instanceID, 0, 100);
	ImGui::SliderInt("Clip", &clip, 0, 1);
	if (ImGui::Button("Play"))
		models->PlayClip(instanceID, clip);

	models->GUIRender();
}
