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
}

Human::~Human()
{
	delete crowbar;
	delete rightHand;
}

void Human::Update()
{
	rightHand->SetWorld((Matrix&)GetTransformByNode(51));

	crowbar->Update();

	__super::Update();
}

void Human::Render()
{
	crowbar->Render();
	__super::Render();
}

void Human::GUIRender()
{
	__super::GUIRender();
	crowbar->GUIRender();
}
