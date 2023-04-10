#include "framework.h"

void PlayerController::Control(void* pack)
{
	MapCursor* mapCursor = (MapCursor*)pack;

	if (mapCursor == nullptr)
		return;

	//플레이어 컨트롤
	if (SRPGUIManager::Get()->IsActing()) {
		if (!SRPGUIManager::Get()->IsMapControl()) {
			mapCursor->UpdateWorld();
			return;
		}
	}
	else {
		if (KEY_DOWN('A')) {
			SRPGUIManager::Get()->OpenUI("MainSelect");
		}
		if (KEY_DOWN('Z')) {
			Observer::Get()->ExcuteEvent("InputAction");
			if (CharacterManager::Get()->HoldedCharacter())
				SRPGUIManager::Get()->OpenUI("ActionSelect");
		}
	}

	mapCursor->Control();
	mapCursor->Update();
}
