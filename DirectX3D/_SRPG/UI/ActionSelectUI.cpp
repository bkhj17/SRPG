#include "framework.h"

ActionSelectUI::ActionSelectUI(Vector3 pos)
	: SelectUI({ 200.0f, 200.0f }, pos)
{
	text = { "이동", "공격", "행동종료", "취소" };
	maxCursor = text.size();
}

void ActionSelectUI::Close()
{
	__super::Close();
	CharacterManager::Get()->CharacterUnhold();
}

void ActionSelectUI::ActiveFunc()
{
	switch (cursor)
	{
	case 0:
		if(CharacterManager::Get()->HoldedCharacter()->IsMoved())
			CharacterManager::Get()->HoldedCharacter()->CancelMove();
		SRPGUIManager::Get()->OpenUI("MapSelectMove");
		break;
	case 1:
		SRPGUIManager::Get()->OpenUI("MapSelectAttack");
		break;
	case 2:
		if(CharacterManager::Get()->HoldedCharacter() != nullptr)
			CharacterManager::Get()->HoldedCharacter()->ActEnd();
		Observer::Get()->ExcuteEvent("CheckMovable");
		Close();
		break;
	default:
		CancelFunc();
		break;
	}
}

void ActionSelectUI::CancelFunc()
{
	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded != nullptr && holded->IsMoved()) {
		CharacterManager::Get()->HoldedCharacter()->CancelMove();
		SRPGUIManager::Get()->OpenUI("MapSelectMove");
	}
	else {
		Close();
	}
}