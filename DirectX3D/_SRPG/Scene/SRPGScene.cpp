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
	
	mapCursor = new MapCursor;
	mapCursor->SetGridTerrain(terrain);
	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&SRPGScene::CharacterMoveEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("InputAction", bind(&SRPGScene::InputAction, this));
	Observer::Get()->AddEvent("InputAttack", bind(&SRPGScene::InputAttackAction, this));

	SRPGUIManager::Get()->OpenUI("TurnChangeUI", Vector3(CENTER_X, CENTER_Y));

	TurnManager::Get();
}

SRPGScene::~SRPGScene()
{
	delete terrain;
	delete mapCursor;

	CharacterManager::Delete();
	SRPGUIManager::Delete();

	TurnManager::Delete();
}

void SRPGScene::Start()
{
	//예뻐지긴 했는데 나중에 뭔가 더 해야할 것 같다
	CharacterManager::Get()->Spawn("test1", Character::Team::PLAYER, terrain, 5, 5);
	CharacterManager::Get()->Spawn("test1-1", Character::Team::PLAYER, terrain, 5, 6);
	CharacterManager::Get()->Spawn("test2", Character::Team::ENEMY, terrain, 6, 8);
	CharacterManager::Get()->Spawn("test2-1", Character::Team::ENEMY, terrain, 6, 7);

	mapCursor->SetPosCoord(5, 6, true);
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
	if (!CharacterManager::Get()->IsActing() && state == PLAYING)
		mapCursor->Render();

	if (state == WIN) {

	}
	else if (state == LOSE) {

	}
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
	if (state != PLAYING)
		return;

	if (CharacterManager::Get()->NumActiveCharactersByTeam(Character::Team::PLAYER) == 0) {
		//플레이어 패배
		state = LOSE;
		SRPGUIManager::Get()->CloseAll();
		SRPGUIManager::Get()->OpenUI("Lose");
		Observer::Get()->ExcuteEvent("GameLose");
		return;
	}
	else if(CharacterManager::Get()->NumActiveCharactersByTeam(Character::Team::ENEMY) == 0) {
		//플레이어 승리
		state = WIN;
		SRPGUIManager::Get()->CloseAll();
		SRPGUIManager::Get()->OpenUI("Win");
		Observer::Get()->ExcuteEvent("GameWin");
		return;
	}

	if (TurnManager::Get()->GetCurPlayer() == Character::Team::PLAYER)
		TurnManager::Get()->Control(mapCursor);
	else
		TurnManager::Get()->Control(terrain);
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
