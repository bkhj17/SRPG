#include "framework.h"
#include "TutorialScene.h"
#include "../Homework/Cube230220.h"

TutorialScene::TutorialScene()
{
	vertexShader = Shader::AddVS(L"Tutorial.hlsl");
	pixelShader = Shader::AddPS(L"Tutorial.hlsl");
	/*
	float size = 1.0f;

	vertices.emplace_back(-size, -size, -size, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(-size, +size, -size, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(+size, -size, -size, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(+size, +size, -size, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(-size, -size, +size, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(-size, +size, +size, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(+size, -size, +size, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(+size, +size, +size, 0.0f, 0.0f, 1.0f);
	
	indices = { 
		//Front
		0, 1, 2, 2, 1, 3,
		//Up
		1, 5, 3, 3, 5, 7,
	};
	
	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	worldBuffer = new MatrixBuffer;
	worldBuffer->SetVS(0);
	*/
	cube = new Cube230220;
}

TutorialScene::~TutorialScene()
{
	/*
	delete vertexBuffer;
	delete indexBuffer;
	delete worldBuffer;
	*/
}

void TutorialScene::Update()
{
	cube->Update();
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
	vertexShader->Set();
	pixelShader->Set();
	/*
	vertexBuffer->Set();
	indexBuffer->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
	*/
	cube->Render();
}

void TutorialScene::PostRender()
{
}

void TutorialScene::GUIRender()
{
	cube->GUIRender();
}
