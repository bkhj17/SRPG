#include "framework.h"

ActionSelectUI::ActionSelectUI(Vector3 pos)
	: SelectUI({ 200.0f, 200.0f }, pos)
{
	text = { "�̵�", "����", "���", "���" };
	maxCursor = (int)text.size();
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
		if (CharacterManager::Get()->HoldedCharacter() == nullptr) {
			Close();
			break;
		}
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