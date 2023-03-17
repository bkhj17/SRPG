#include "framework.h"
#include "Crowbar.h"

Crowbar::Crowbar()
	: Model("crowbar")
{
	SetTag("Crowbar");
	Load();

	collider = new BoxCollider();
	collider->SetTag("CrowbarCollider");
	collider->SetParent(this);
	collider->Load();
}

Crowbar::~Crowbar()
{
	delete collider;
}

void Crowbar::Update()
{
	UpdateWorld();
	collider->UpdateWorld();
}

void Crowbar::Render()
{
	__super::Render();
	collider->Render();
}

void Crowbar::GUIRender()
{


	Model::GUIRender();
	collider->GUIRender();
}
