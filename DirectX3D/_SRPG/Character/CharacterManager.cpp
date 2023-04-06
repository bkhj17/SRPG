#include "framework.h"
#include "CharacterManager.h"
#include "Character.h"
#include "../Field/GridedTerrain.h"

CharacterManager::CharacterManager()
{
	vector<pair<int, string>> v = {
		{Character::Team::PLAYER, "Soldier"},
		{Character::Team::ENEMY, "Enemy"}
	};
	for (auto& p : v) {
		characterPool[p.first].resize(MAX_POOL);
		for (auto& character : characterPool[p.first] ) {
			character = new Character(p.second);
			character->SetActive(false);
		}
	}

	Observer::Get()->AddEvent("CharacterUnhold", bind(&CharacterManager::CharacterUnhold, this));
	Observer::Get()->AddParamEvent("CharacterAttackHit", bind(&CharacterManager::AttackHit, this, placeholders::_1));
	Observer::Get()->AddParamEvent("CharacterAttackEnd", bind(&CharacterManager::AttackEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("TurnStart", bind(&CharacterManager::TurnStart, this));
}

CharacterManager::~CharacterManager()
{
	for (auto& pool : characterPool) {
		for (auto character : pool.second)
			delete character;
		pool.second.clear();
	}	
	characterPool.clear();
}

void CharacterManager::Update()
{
	for (auto& pool : characterPool)
		for (auto character : pool.second)
			character->Update();

	if (isBattle)
		BattleUpdate();
}

void CharacterManager::Render()
{
	for (auto& pool : characterPool)
		for (auto character : pool.second)
			character->Render();
}

void CharacterManager::PostRender()
{
	for (auto& pool : characterPool)
		for (auto character : pool.second)
			character->PostRender();
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

	for (auto& pool : characterPool) {
		for (auto character : pool.second) {
			if (character->IsActing())
				return true;
		}
	}

	return false;
}

Character* CharacterManager::Spawn(int teamNum)
{
	for (auto character : characterPool[teamNum]) {
		if (character->Active())
			continue;

		character->Init();
		return character;
	}

	return nullptr;
}

Character* CharacterManager::Spawn(GridedTerrain* terrain, int teamNum, int w, int h)
{
	assert(terrain != nullptr);

	Character* spawned = Spawn(teamNum);
	if (spawned == nullptr)
		return nullptr;

	spawned->originPos = spawned->Pos() = terrain->CoordToPos(w, h);
	spawned->UpdateWorld();
	terrain->AddObject(spawned);
	return spawned;
}

Character* CharacterManager::Spawn(string name, int teamNum, GridedTerrain* terrain, int w, int h)
{
	Character* character = Spawn(terrain, teamNum, w, h);
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
	

	//사거리에 따라 공격 불가능하게도 해야 한다
	pair<Vector3, pair<int, int>> pack = {};
	pack.first = offense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);

	pair<int, int> coordO = pack.second;			//공격자 위치 coord

	pack.first = defense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);
	pair<int, int> coordD = pack.second;

	int dist = abs(coordO.first - coordD.first) + abs(coordO.second - coordD.second);

	//방어자가 공격자 사거리 밖 => 애초에 사거리 밖이라 전투 자체가 성립 안 함
	if (offense->GetAttackRange().first > dist || offense->GetAttackRange().second < dist)
		return;

	//스테이터스에 따라 미리 데미지 설정
	attacks.push({ CalcDamage(offense, defense), offense, defense });		//공격자 공격
	
	//공격자가 방어자 사거리 안에 있을 때에만 반격을 실행
	if (defense->GetAttackRange().first <= dist && defense->GetAttackRange().second >= dist)
		attacks.push({ CalcDamage(defense, offense),defense, offense });	//방어자 반격


	//전투가 성립했다
	isBattle = !attacks.empty();
	curOffense = offense;
	curDefense = defense;


	//카메라 설정
}

void CharacterManager::TurnStart()
{

	for (auto& pool : characterPool) {
		for (auto character : pool.second) {
			if (character->Active())
				character->TurnStart();
		}
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
	for (auto& pool : characterPool) {
		for (auto character : pool.second) {
			if (!character->Active() || character->IsActed())
				continue;

			if (character->GetStatus().teamNum == team)
				return character;
		}
	}
	return nullptr;
}

int CharacterManager::NumActiveCharactersByTeam(Character::Team team)
{
	int cnt = 0;
	for (auto character : characterPool[team]) {
		if (!character->Active())
			continue;

		cnt++;
	}
	return cnt;
}

void CharacterManager::BattleUpdate()
{
	if (attacks.empty() || !curOffense->Active() || !curDefense->Active()) {
		BattleEnd();
		return;
	}

	auto attacker = attacks.front().attacker;
	auto defender = attacks.front().hit;
	if (!attacker->IsActing() && !defender->IsActing()) {
		//공격 아직 시작 안 됨 -> 시작명령
		//공격 방향
		Vector3 hitPos = defender->Pos();
		Vector3 dir = defender->Pos() - attacker->Pos();
		dir.y = 0.0f;
		attacker->SetDir(dir);
		//defender->SetDir(-dir); //기습 당하는 느낌인게 좋겠다 싶어 제외

		attacker->SetAttackAnim();
	}
}

void CharacterManager::BattleEnd()
{
	curOffense->ActEnd();

	isBattle = false;
	curOffense = nullptr;
	curDefense = nullptr;

	while(!attacks.empty())
		attacks.pop();

	//카메라 설정
	Observer::Get()->ExcuteEvent("BattleEnd");
	CharacterUnhold();
}

void CharacterManager::AttackHit(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (attacks.empty() || attacks.front().attacker != attacked)
		return;

	ParticleManager::Get()->Play("Hit", attacks.front().hit->GlobalPos() + Vector3(0, 2.0f, 0));

	attacks.front().hit->Damaged(attacks.front().damage);
}

void CharacterManager::AttackEnd(void* ptr)
{
	auto attacked = (Character*)ptr;
	if (!attacks.empty() && attacks.front().attacker == attacked)
		attacks.pop();
}

int CharacterManager::CalcDamage(Character* attacker, Character* defender)
{
	//공격 = 힘 + 무기 위력 * (유효 ? 2 : 1) 
	int attack = attacker->CalcAttack();
	int defence = defender->GetStatus().defence;

	//피해량 = (공격 - 수비)*(1 or 3(크리티컬) )
	int damage = attack - defence;

	return damage;
}
