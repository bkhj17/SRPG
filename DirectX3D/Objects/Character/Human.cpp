#include "framework.h"

Human::Human()
	: ModelAnimator("Human")
{
	crowbar = new Crowbar();

	ReadClip("idle", 1);
	ReadClip("run", 1);
	ReadClip("jump", 1);

	rightHand = new Transform();
	crowbar->SetParent(rightHand);

	startEdge = new Transform();
	endEdge = new Transform();

	trail = new Trail(L"Textures/Effect/Trail.png", startEdge, endEdge, 50, 100.0f);
}

Human::~Human()
{
	delete crowbar;
	delete rightHand;
	delete startEdge;
	delete endEdge;

	delete trail;
}

void Human::Update()
{
	__super::Update();
	
	rightHand->SetWorld((Matrix&)GetTransformByNode(51));
	crowbar->Update();

	startEdge->Pos() = crowbar->GlobalPos() + crowbar->Up() * 2.0f;
	startEdge->UpdateWorld();
	endEdge->Pos() = crowbar->GlobalPos() - crowbar->Up();
	endEdge->UpdateWorld();

	trail->Update();
}

void Human::Render()
{
	crowbar->Render();
	__super::Render();

	trail->Render();
}

void Human::GUIRender()
{
	__super::GUIRender();
	crowbar->GUIRender();
}
