#include "framework.h"
#include "Player230308.h"

void Player230308::Update()
{
	Control();
	Jump();


	Observer::Get()->ExcuteParamEvent("GetHeight", this);
	Pos().y += 1.0f * GlobalScale().y;

	UpdateWorld();

	mineUI->Update();
}

