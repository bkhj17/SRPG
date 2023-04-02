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
	CharacterManager::Get()->Spawn("test1", Character::Team::PLAYER, terrain, 5, 5);
	CharacterManager::Get()->Spawn("test2", Character::Team::ENEMY, terrain, 6, 8);

	mapCursor = new MapCursor;
	mapCursor->SetGridTerrain(terrain);
	mapCursor->SetPosCoord(5, 6, true);
	Observer::Get()->AddParamEvent("CharacterMoveEnd", bind(&SRPGScene::CharacterMoveEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("InputAction", bind(&SRPGScene::InputAction, this));
	Observer::Get()->AddEvent("InputAttack", bind(&SRPGScene::InputAttackAction, this));

	SRPGUIManager::Get()->OpenUI("Test", Vector3(CENTER_X - 300.0f, CENTER_Y));

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
	if (TurnManager::Get()->GetCurPlayer() == Character::Team::PLAYER) {
		//플레이어 컨트롤
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
	}
	else {
		//에너미 컨트롤
		//홀드된 캐릭터가 있다
		auto holded = CharacterManager::Get()->HoldedCharacter();
		if(holded) {
			if (holded->IsActed())
				CharacterManager::Get()->CharacterUnhold();
			int holdedIndex = terrain->CoordToIndex(terrain->PosToCoord(holded->GlobalPos()));
			//공격범위 내의 적들 찾기
			auto vAttackables = terrain->AttackableCharacters(Character::Team::PLAYER);
			if (vAttackables.empty()) {
				holded->ActEnd();
				CharacterManager::Get()->CharacterUnhold();
				return;
			}
			
			//가장 가까운 적과 그 적을 공격할 위치
			Character* target = vAttackables.front().first;
			int tile = vAttackables.front().second.second;
			//사거리 내에 적이 있다 == 현재 위치가 적을 공격할 위치
			if(tile == holdedIndex)
			{

				//해당 적을 타겟으로 전투
				auto coord = terrain->PosToCoord(target->GlobalPos());
				terrain->InputAction(coord.first, coord.second, GridedTerrain::ATTACK);
			}
			else {
				if(holded->IsMoved())
				{
					//이동 했다면
					CharacterManager::Get()->CharacterUnhold();
				}
				else {
					//이동 안 했다면
					//가장 가까운 적을 향해
					//이동
					auto coord = terrain->IndexToCoord(tile);
					terrain->InputAction(coord.first, coord.second);
				}
			}
			return;
		}

		//자기편 캐릭터 중 안 움직인 캐릭터 찾기
		Character* character = CharacterManager::Get()->GetActableCharacter(Character::Team::ENEMY);
		
		//행동 안 한 캐릭터 없으면
		if(character == nullptr)
		{
			//턴 엔드
			TurnManager::Get()->NextTurn();
			return;
		}
		else {
			//있으면 해당 캐릭터 선택
			auto coord = terrain->PosToCoord(character->Pos());
			terrain->SetSelected(coord.first, coord.second, true);
			terrain->InputAction();
		}
	}
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
