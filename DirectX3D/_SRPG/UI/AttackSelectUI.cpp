#include "framework.h"
#include "AttackSelectUI.h"

AttackSelectUI::AttackSelectUI()
	: MapSelectUI("InputAttack")
{
}

void AttackSelectUI::Init()
{
	Observer::Get()->ExcuteEvent("SetStandingAttack");
	__super::Init();
}

void AttackSelectUI::Close()
{
	Observer::Get()->ExcuteEvent("UnsetStandingAttack");
	__super::Close();
}
