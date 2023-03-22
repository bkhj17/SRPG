#include "framework.h"
#include "GameScene.h"

GameScene::GameScene()
{
	forest = new Model("Forest");
	forest->Scale() *= 10.0f;
	forest->UpdateWorld();
	forest->SetShader(L"PostEffect/Fog.hlsl");

	fogBuffer = new FogBuffer();

	naruto = new Naruto;

	blendState[0] = new BlendState;
	blendState[1] = new BlendState;
	blendState[1]->AlphaToCoverage(true);

	CAM->SetTarget(naruto);
	CAM->TargetOptionLoad("Naruto");
	CAM->LookAtTarget();

	KunaiManager::Get();
	RobotManager::Get()->SetTarget(naruto);

	skyBox = new SkyBox(L"Textures/LandScape/Snow_ENV.dds");

	Audio::Get()->Add("bgm", "Sounds/naruto.mp3", true, true);
	Audio::Get()->Add("move", "Sounds/move.wav", false, true);
	Audio::Get()->Add("hit", "Sounds/hit.wav", false, false);
	//Audio::Get()->Play("bgm");
}

GameScene::~GameScene()
{
	delete forest;
	delete naruto;

	delete blendState[0];
	delete blendState[1];

	KunaiManager::Delete();
	RobotManager::Delete();

	delete skyBox;
}

void GameScene::Update()
{
	naruto->Update();

	KunaiManager::Get()->Update();
	RobotManager::Get()->Update();
}

void GameScene::PreRender()
{
}

void GameScene::Render()
{
	skyBox->Render();

	naruto->Render();

	blendState[1]->SetState();
	fogBuffer->SetPS(10);
	forest->Render();
	blendState[0]->SetState();

	KunaiManager::Get()->Render();
	RobotManager::Get()->Render();
}

void GameScene::PostRender()
{
	RobotManager::Get()->PostRender();
	naruto->PostRender();
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
