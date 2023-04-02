#include "framework.h"
#include "PlayerController.h"

PlayerController::PlayerController()
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Control()
{
	/*
	//mapCursor만 어떻게 할 수 있으면 되는데...
	//포기
	if (SRPGUIManager::Get()->IsActing()) {
		if (!SRPGUIManager::Get()->IsMapControl()) {
			mapCursor->UpdateWorld();
			return;
		}
	}
	else {
		if (KEY_DOWN('Z')) {
			InputAction();
			if (CharacterManager::Get()->HoldedCharacter())
				SRPGUIManager::Get()->OpenUI("ActionSelect");
		}
	}

	mapCursor->Control();
	mapCursor->Update(); 
	*/
}
