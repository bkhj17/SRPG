#include "framework.h"
#include "DeferredScene.h"

DeferredScene::DeferredScene()
{
	gBuffer = new GBuffer;

	forest = new Model("Forest");
	human = new Human;
}

DeferredScene::~DeferredScene()
{
	delete gBuffer;

	delete forest;
	delete human;
}

void DeferredScene::Update()
{
	forest->UpdateWorld();
	human->Update();
}

void DeferredScene::PreRender()
{
	gBuffer->PreRender();

	forest->SetShader(L"Light/GBuffer.hlsl");
	human->SetShader(L"Light/AnimGBuffer.hlsl");
	human->GetCrowbar()->SetShader(L"Light/GBuffer.hlsl");

	forest->Render();
	human->Render();
}

void DeferredScene::Render()
{
}

void DeferredScene::PostRender()
{
	gBuffer->PostRender();
}

void DeferredScene::GUIRender()
{
}
