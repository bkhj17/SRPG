#include "framework.h"
#include "SRPGScene.h"
#include "../Field/GridedTerrain.h"
#include "../Field/MapCursor.h"
#include "../Character/Character.h"
#include "../Character/CharacterManager.h"
#include "../UI/SRPGUIManager.h"
#include "../UI/UIWindow.h"

SRPGScene::SRPGScene()
{
	terrain = new GridedTerrain;
	
	//예뻐지긴 했는데 나중에 뭔가 더 해야할 것 같다
	CharacterManager::Get()->Spawn(terrain, 5, 5);
	CharacterManager::Get()->Spawn(terrain, 6, 8);

	mapCursor = new MapCursor;
	mapCursor->SetGridTerrain(terrain);
	mapCursor->SetPosCoord(5, 6, true);
	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&SRPGScene::CharacterMoveEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("InputAction", bind(&SRPGScene::InputAction, this));
	Observer::Get()->AddEvent("InputAttack", bind(&SRPGScene::InputAttackAction, this));

	SRPGUIManager::Get()->OpenUI("Test", Vector3(CENTER_X - 300.0f, CENTER_Y));
}

SRPGScene::~SRPGScene()
{
	delete terrain;
	delete mapCursor;

	CharacterManager::Delete();
	SRPGUIManager::Delete();
}

void SRPGScene::Update()
{
	terrain->Update();
	CharacterManager::Get()->Update();

	SRPGUIManager::Get()->Update();
	if (!CharacterManager::Get()->IsActing())
		Control();
}

void SRPGScene::PreRender()
{
}

void SRPGScene::Render()
{
	terrain->Render();

	CharacterManager::Get()->Render();
	if (!CharacterManager::Get()->IsActing())
		mapCursor->Render();
}

void SRPGScene::PostRender()
{
	SRPGUIManager::Get()->Render();
}

void SRPGScene::GUIRender()
{
	terrain->GUIRender();
}

void SRPGScene::Control()
{
	if (SRPGUIManager::Get()->IsActing()) {
		if (!SRPGUIManager::Get()->IsMapControl())
			return;
	}
	else {
		if (KEY_DOWN('Z')) {
			InputAction();
			if (CharacterManager::Get()->HoldedCharacter())
				SRPGUIManager::Get()->OpenUI("ActionSelect");
		}
	}

	mapCursor->Update();
}

void SRPGScene::InputAction()
{
	terrain->InputAction(mapCursor->GetW(), mapCursor->GetH());
}

void SRPGScene::InputAttackAction()
{
	terrain->InputAction(mapCursor->GetW(), mapCursor->GetH(), GridedTerrain::ATTACK);
}

void SRPGScene::CharacterMoveEnd(void* characterPtr)
{
	auto character = (Character*)characterPtr;
	if (!character) //wrong call
		return;

	pair<int, int> characterPosCoord = terrain->PosToCoord(character->GlobalPos());
	mapCursor->SetPosCoord(characterPosCoord.first, characterPosCoord.second, true);
	terrain->CheckMovableArea();

	SRPGUIManager::Get()->CloseUI("MapSelectMove");
}
