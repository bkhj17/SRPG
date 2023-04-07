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
	
	WeaponManager::Get();
	CharacterManager::Get();

	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&SRPGScene::CharacterMoveEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("InputAction", bind(&SRPGScene::InputAction, this));
	Observer::Get()->AddEvent("InputAttack", bind(&SRPGScene::InputAttackAction, this));

	SRPGUIManager::Get()->OpenUI("TurnChangeUI", Vector3(CENTER_X, CENTER_Y));

	TurnManager::Get();

	ParticleManager::Get()->Add("Hit", "TextData/Particles/Hit.fx", 10);

	battleCamera = new BattleCameraMan;
	battleCamera->Pos() = mapCursor->GlobalPos();
}

SRPGScene::~SRPGScene()
{
	delete terrain;
	delete mapCursor;
	delete battleCamera;

	CharacterManager::Delete();
	WeaponManager::Delete();

	SRPGUIManager::Delete();

	TurnManager::Delete();

	ParticleManager::Delete();
}

void SRPGScene::Start()
{
	//예뻐지긴 했는데 나중에 뭔가 더 해야할 것 같다
	Character* character = CharacterManager::Get()->Spawn("test1", Character::Team::PLAYER, terrain, 3, 3);
	character->SetWeapon(WeaponManager::Get()->Pop("Axe"));
	
	character = CharacterManager::Get()->Spawn("test1-1", Character::Team::PLAYER, terrain, 4, 3);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));
	
	character = CharacterManager::Get()->Spawn("test1-2", Character::Team::PLAYER, terrain, 3, 10);
	character->SetWeapon(WeaponManager::Get()->Pop("Bow"), 12);
	
	character = CharacterManager::Get()->Spawn("test2", Character::Team::ENEMY, terrain, 7, 10);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));

	character = CharacterManager::Get()->Spawn("test2-1", Character::Team::ENEMY, terrain, 9, 10);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));

	mapCursor->SetPosCoord(5, 6, true);
}

void SRPGScene::Update()
{
	terrain->Update();
	CharacterManager::Get()->Update();
	WeaponManager::Get()->Update();


	SRPGUIManager::Get()->Update();
	if (!CharacterManager::Get()->IsActing())
		Control();

	if (CharacterManager::Get()->IsBattle()) {
		battleCamera->Pos() = CharacterManager::Get()->HoldedCharacter()->GlobalPos();
		battleCamera->Rot() = CharacterManager::Get()->HoldedCharacter()->Rot();
		battleCamera->Update();
	}

	ParticleManager::Get()->Update();
}

void SRPGScene::PreRender()
{
}

void SRPGScene::Render()
{
	if (CharacterManager::Get()->IsBattle())
		battleCamera->SetView();

	terrain->Render();

	CharacterManager::Get()->Render();
	WeaponManager::Get()->Render();
	
	if (!CharacterManager::Get()->IsActing() && state == PLAYING)
		mapCursor->Render();

	ParticleManager::Get()->Render();
}

void SRPGScene::PostRender()
{
	
	CharacterManager::Get()->PostRender();

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
