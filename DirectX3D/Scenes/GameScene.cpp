#include "framework.h"
#include "GameScene.h"

GameScene::GameScene()
{
	forest = new Model("Forest");
	forest->SetShader(L"Basic/Texture.hlsl");
	forest->Scale() *= 10.0f;
	forest->UpdateWorld();

	naruto = new Naruto;


	blendState[0] = new BlendState;
	blendState[1] = new BlendState;
	blendState[1]->AlphaToCoverage(true);

	CAM->SetTarget(naruto);
	CAM->TargetOptionLoad("Naruto");
	CAM->LookAtTarget();

	KunaiManager::Get();
}

GameScene::~GameScene()
{
	delete forest;
	delete naruto;

	delete blendState[0];
	delete blendState[1];

	KunaiManager::Delete();
}

void GameScene::Update()
{
	naruto->Update();

	KunaiManager::Get()->Update();
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
	naruto->Render();

	forest->Render();

	KunaiManager::Get()->Render();
}

void GameScene::PostRender()
{
}

void GameScene::GUIRender()
{
	naruto->GUIRender();
}

void GameScene::Start()
{
}

void GameScene::End()
{
}
