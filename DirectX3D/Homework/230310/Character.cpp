#include "framework.h"
#include "Character.h"

Character::Character()
{
	cube = new Cube;
	cube->SetParent(this);
	cube->Pos().y = 1.0f;
}

Character::~Character()
{
	delete cube;
}

void Character::Update()
{
	UpdateWorld();
	cube->Update();
}

void Character::Render()
{
	cube->Render();
}
