#include "framework.h"

Weapon::~Weapon()
{
	data = nullptr;
	modelTransform = nullptr;
	owner = nullptr;
}

void Weapon::Update()
{
	if (owner) {
		SetWorld(owner->GetWorld());
	}
	else
		UpdateWorld();

	if (modelTransform)
		modelTransform->UpdateWorld();
}

void Weapon::SetModelTransform(string tag, Transform* transform)
{
	if (modelTransform) {
		modelTransform->SetParent(nullptr);
		modelTransform->SetActive(false);
	}

	if (transform) {
		transform->Load(tag);
		transform->SetParent(this);
		transform->SetActive(true);
	}
	modelTransform = transform;
}
