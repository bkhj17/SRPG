#include "framework.h"
#include "CharacterManager.h"
#include "Character.h"
#include "../Field/GridedTerrain.h"

CharacterManager::CharacterManager()
{
	characterPool.resize(MAX_POOL);
	for (auto& character : characterPool) {
		character = new Character();
		character->SetActive(false);
	}

	Observer::Get()->AddEvent("CharacterUnhold", bind(&CharacterManager::CharacterUnhold, this));
	Observer::Get()->AddParamEvent("CharacterAttackHit", bind(&CharacterManager::AttackHit, this, placeholders::_1));
	Observer::Get()->AddParamEvent("CharacterAttackEnd", bind(&CharacterManager::AttackEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("TurnStart", bind(&CharacterManager::TurnStart, this));

}

CharacterManager::~CharacterManager()
{
	for (auto character : characterPool)
		delete character;
	characterPool.clear();
}

void CharacterManager::Update()
{
	for (auto character : characterPool)
		character->Update();

	if (isBattle)
		BattleUpdate();
}

void CharacterManager::Render()
{
	for (auto character : characterPool)
		character->Render();
}

void CharacterManager::CharacterUnhold()
{
	if (holded == nullptr)
		return;

	if (holded->moved)
		holded->ActEnd();
	
	CharacterHold(nullptr, -1, -1);
}

bool CharacterManager::IsActing()
{
	if (isBattle)
		return true;

	for (auto character : characterPool) {
		if (character->IsActing())
			return true;
	}

	return false;
}

Character* CharacterManager::Spawn()
{
	for (auto character : characterPool) {
		if (character->Active())
			continue;

		character->SetActive(true);
		return character;
	}
	return nullptr;
}

Character* CharacterManager::Spawn(GridedTerrain* terrain, int w, int h)
{
	assert(terrain != nullptr);

	Character* spawned = Spawn();
	if (spawned == nullptr)
		return nullptr;

	spawned->originPos = spawned->Pos() = terrain->CoordToPos(w, h);
	spawned->UpdateWorld();
	terrain->AddObject(spawned);
	return spawned;
}

Character* CharacterManager::Spawn(string name, int teamNum, GridedTerrain* terrain, int w, int h)
{
	Character* character = Spawn(terrain, w, h);
	if(character == nullptr)
		return nullptr;

	character->status.name = name;
	character->status.teamNum = teamNum;
	return character;
}

void CharacterManager::BattleStart(Character* offense, Character* defense)
{
	if (!offense || !offense->Active()
		|| !defense || !defense->Active())
		return;
	
	isBattle = true;
	curOffense = offense;
	curDefense = defense;
	//스테이터스에 따라 공방 설정
	attacks.push({ 1, offense, defense });
	attacks.push({ 1, defense, offense });

	//카메라 설정
}

void CharacterManager::TurnStart()
{
	for (auto character : characterPool) {
		if (character->Active())
			character->TurnStart();
	}
}

void CharacterManager::CancelMove()
{
	if (holded == nullptr)
		return;

	holded->CancelMove();
}

Character* CharacterManager::GetActableCharacter(Character::Team team)
{
	for (auto character : characterPool) {
		if (!character->Active() || character->IsActed())
			continue;

		if (character->GetStatus().teamNum == team)
			return character;
	}
	return nullptr;
}

void CharacterManager::BattleUpdate()
{
	if (attacks.empty() || !curOffense->Active() || !curDefense->Active()) {
		BattleEnd();
		return;
	}

	auto attacker = attacks.front().attacker;
	auto defender = attacks.front().hit;
	if (!attacker->IsActing()) {
		//공격 아직 시작 안 됨 -> 시작명령
		//공격 방향
		Vector3 hitPos = defender->Pos();
		Vector3 attackerPos = attacker->Pos();
		Vector3 dir = defender->Pos() - attacker->Pos();
		dir.y = 0.0f;
		attacker->SetDir(dir);
		//defender->SetDir(-dir); //기습 당하는 느낌인게 좋겠다 싶어 제외

		attacker->SetAnimState(Character::ATTACK);
	}
}

void CharacterManager::BattleEnd()
{
	curOffense->ActEnd();

	isBattle = false;
	curOffense = nullptr;
	curDefense = nullptr;
		
	//카메라 설정
	Observer::Get()->ExcuteEvent("BattleEnd");
	CharacterUnhold();
}

void CharacterManager::AttackHit(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (attacks.empty() || attacks.front().attacker != attacked)
		return;

	attacks.front().hit->Damaged(1);
}

void CharacterManager::AttackEnd(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (!attacks.empty() && attacks.front().attacker == attacked)
		attacks.pop();
}
