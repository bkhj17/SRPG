#include "framework.h"
#include "Weapon.h"

Weapon::Weapon(string tag)
{
	model = new Model(tag);
	model->SetTag(tag);
	model->Load();
	model->SetParent(this);
}

Weapon::~Weapon()
{
	delete model;
}

void Weapon::Update()
{
	if (owner)
		SetWorld(owner->GetTransformByNode(boneNum));
	else
		UpdateWorld();

	model->UpdateWorld();
}

void Weapon::Render()
{
	model->Render();
}
