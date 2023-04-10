#include "framework.h"
#include "SRPGScene.h"

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
	ParticleManager::Get()->Add("Hit", "TextData/Particles/ObstacleBreak.fx", 10);
	
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
	//장애물 세팅
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 3, 6);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 4, 6);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 5, 6);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 6, 6);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 7, 6);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 8, 7);
	CharacterManager::Get()->Spawn("Box", Character::Team::NONE, terrain, 9, 7);

	//캐릭터 세팅
	//데이터화...는 딱히?
	Character* character = (Character*)CharacterManager::Get()->Spawn("Soldier1", Character::Team::PLAYER, terrain, 3, 5);
	character->SetWeapon(WeaponManager::Get()->Pop("Axe"));
	
	character = (Character*)CharacterManager::Get()->Spawn("Soldier2", Character::Team::PLAYER, terrain, 4, 3);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));
	
	character = (Character*)CharacterManager::Get()->Spawn("Soldier3", Character::Team::PLAYER, terrain, 3, 3);
	character->SetWeapon(WeaponManager::Get()->Pop("Bow"), 12);

	character = (Character*)CharacterManager::Get()->Spawn("Soldier2", Character::Team::PLAYER, terrain, 4, 4);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));

	character = (Character*)CharacterManager::Get()->Spawn("Enemy1", Character::Team::ENEMY, terrain, 7, 10);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));

	character = (Character*)CharacterManager::Get()->Spawn("Enemy2", Character::Team::ENEMY, terrain, 9, 10);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));

	character = (Character*)CharacterManager::Get()->Spawn("Enemy3", Character::Team::ENEMY, terrain, 8, 9);
	character->SetWeapon(WeaponManager::Get()->Pop("Axe"));

	character = (Character*)CharacterManager::Get()->Spawn("Enemy4", Character::Team::ENEMY, terrain, 6, 9);
	character->SetWeapon(WeaponManager::Get()->Pop("Sword"));
	//커서 초기 위치
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
