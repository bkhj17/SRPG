#include "framework.h"

CharacterManager::CharacterManager()
{
	vector<pair<int, string>> v = {
		{Character::Team::PLAYER, "Soldier"},
		{Character::Team::ENEMY, "Enemy"}
	};
	for (auto& p : v) {
		instances[p.first] = new ModelAnimatorInstancing(p.second);
		instances[p.first]->ReadClip("SwordIdle");
		instances[p.first]->ReadClip("Run");
		instances[p.first]->ReadClip("Hit");
		instances[p.first]->ReadClip("Death");
		instances[p.first]->ReadClip("SwordAttack");
		instances[p.first]->ReadClip("BowAttack");

		objectPool[p.first].resize(MAX_POOL);

		for (auto& character : objectPool[p.first] ) {
			character = new Character(instances[p.first]);
			character->SetActive(false);
		}
	}

	objectPool[SRPGObject::Team::NONE].resize(MAX_POOL);
	for (auto& object : objectPool[SRPGObject::Team::NONE]) {
		object = new Obstacle();
		object->SetActive(false);
	}

	Observer::Get()->AddEvent("CharacterUnhold", bind(&CharacterManager::CharacterUnhold, this));
	Observer::Get()->AddParamEvent("CharacterAttackHit", bind(&CharacterManager::AttackHit, this, placeholders::_1));
	Observer::Get()->AddParamEvent("CharacterAttackEnd", bind(&CharacterManager::AttackEnd, this, placeholders::_1));

	Observer::Get()->AddEvent("TurnStart", bind(&CharacterManager::TurnStart, this));
}

CharacterManager::~CharacterManager()
{
	for (auto& pool : objectPool) {
		for (auto object : pool.second)
			delete object;
	}	

	for (auto& instance : instances)
		delete instance.second;
}

void CharacterManager::Update()
{
	for (auto& instance : instances)
		instance.second->Update();
	for (auto& pool : objectPool)
		for (auto character : pool.second)
			character->Update();

	if (isBattle)
		BattleUpdate();
}

void CharacterManager::Render()
{
	for (auto& instance : instances)
		instance.second->Render();

	for (auto& pool : objectPool)
		for (auto object : pool.second)
			object->Render();
}

void CharacterManager::PostRender()
{
	for (auto& pool : objectPool)
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

	for (auto& pool : objectPool) {
		if (pool.first == SRPGObject::Team::NONE)
			continue;

		for (auto character : pool.second) {
			if (character->IsActing())
				return true;
		}
	}

	return false;
}

SRPGObject* CharacterManager::Spawn(int teamNum)
{
	for (auto object : objectPool[teamNum]) {
		if (object->Active())
			continue;

		object->Init();
		return object;
	}

	return nullptr;
}

SRPGObject* CharacterManager::Spawn(GridedTerrain* terrain, int teamNum, int w, int h)
{
	assert(terrain != nullptr);

	SRPGObject* spawned = Spawn(teamNum);
	if (spawned == nullptr)
		return nullptr;

	spawned->originPos = spawned->Pos() = terrain->CoordToPos(w, h);
	spawned->UpdateWorld();
	terrain->AddObject(spawned);
	return spawned;
}

SRPGObject* CharacterManager::Spawn(string name, int teamNum, GridedTerrain* terrain, int w, int h)
{
	SRPGObject* spawned = Spawn(terrain, teamNum, w, h);
	if(spawned == nullptr)
		return nullptr;

	spawned->status.name = name;
	spawned->status.teamNum = teamNum;
	return spawned;
}

void CharacterManager::BattleStart(SRPGObject* offense, SRPGObject* defense)
{
	auto offenseCharacter = (Character*)offense;

	if (!offenseCharacter || !offenseCharacter->Active()
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
	if (offenseCharacter->GetAttackRange().first > dist || offenseCharacter->GetAttackRange().second < dist)
		return;

	//스테이터스에 따라 미리 데미지 설정
	attacks.push({ CalcDamage(offenseCharacter, defense), offense, defense });		//공격자 공격
	
	//방어자가 캐릭터였을 경우 => 반격
	Character* defenseCharacter = dynamic_cast<Character*>(defense);
	//공격자가 방어자 사거리 안에 있을 때에만 반격을 실행
	if (defenseCharacter != nullptr) {
		if (defenseCharacter->GetAttackRange().first <= dist && defenseCharacter->GetAttackRange().second >= dist)
			attacks.push({ CalcDamage(defenseCharacter, offense), defenseCharacter, offense });	//방어자 반격
	}

	//전투가 성립했다
	isBattle = !attacks.empty();
	curOffense = offense;
	curDefense = defense;


	//카메라 설정
}

void CharacterManager::TurnStart()
{
	for (auto& pool : objectPool) {
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
	for (auto& pool : objectPool) {
		for (auto object : pool.second) {
			auto character = (Character*)object;

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
	for (auto character : objectPool[team]) {
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

	auto attacker = (Character*)attacks.front().attacker;
	auto defender = attacks.front().hit;
	if (!attacker->IsActing() && !defender->IsActing())
		attacker->AttackStart(defender);
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

int CharacterManager::CalcDamage(Character* attacker, SRPGObject* defender)
{
	//공격 = 힘 + 무기 위력 * (유효 ? 2 : 1) 
	int attack = attacker->CalcAttack();
	int defence = defender->GetStatus().defence;

	//피해량 = (공격 - 수비)*(1 or 3(크리티컬) )
	int damage = attack - defence;

	return damage;
}
