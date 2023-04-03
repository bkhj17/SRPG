#include "framework.h"
#include "EnemyController.h"

void EnemyController::Control(void* pack)
{
	if (SRPGUIManager::Get()->IsActing())
		return;

	auto terrain = (GridedTerrain*)pack;
	if (terrain == nullptr)	//사실은 assert가 필요하다
		return;

	//에너미 컨트롤
	auto holded = CharacterManager::Get()->HoldedCharacter();
	//이미 홀드된 캐릭터가 있다
	if (holded) {
		if (holded->IsActed())
			CharacterManager::Get()->CharacterUnhold();
		int holdedIndex = terrain->CoordToIndex(terrain->PosToCoord(holded->GlobalPos()));
		//공격범위 내의 적들 찾기
		auto vAttackables = terrain->AttackableCharacters(Character::Team::PLAYER);
		if (vAttackables.empty()) {
			//공격 범위에 적이 없다
			holded->ActEnd();							//행동 종료
			CharacterManager::Get()->CharacterUnhold();	//및 선택 해제
			return;
		}

		//가장 가까운 적과 그 적을 공격할 위치
		Character* target = vAttackables.front().first;
		int tile = vAttackables.front().second.second;
		//현재 사거리 내에 적이 있다 == 현재 위치가 적을 공격할 위치
		if (tile == holdedIndex)
		{
			//해당 적을 타겟으로 전투
			auto coord = terrain->PosToCoord(target->GlobalPos());
			terrain->InputAction(coord.first, coord.second, GridedTerrain::ATTACK);
		}
		else {
			if (holded->IsMoved())
			{
				//이동 했다면
				CharacterManager::Get()->CharacterUnhold();
			}
			else {
				//이동 안 했다면 가장 가까운 적을 향해 이동
				auto coord = terrain->IndexToCoord(tile);
				terrain->InputAction(coord.first, coord.second);
			}
		}
		return;
	}
	//홀드된 게 없다면
	//자기편 캐릭터 중 안 움직인 캐릭터 찾기
	Character* character = CharacterManager::Get()->GetActableCharacter(Character::Team::ENEMY);
	//행동 안 한 캐릭터 없으면
	if (character == nullptr)
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
