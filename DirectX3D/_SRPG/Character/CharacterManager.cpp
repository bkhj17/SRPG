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

void CharacterManager::Render(bool shadow)
{
	for (auto& instance : instances)
		instance.second->Render();

	if (shadow) {
		for (auto object : objectPool[SRPGObject::NONE])
			object->Render();
	}
	else {
		for (auto& pool : objectPool)
			for (auto object : pool.second)
				object->Render();
	}
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

	//��Ÿ��� ���� ���� �Ұ����ϰԵ� �ؾ� �Ѵ�
	pair<Vector3, pair<int, int>> pack = {};
	pack.first = offense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);

	pair<int, int> coordO = pack.second;			//������ ��ġ coord

	pack.first = defense->Pos();
	Observer::Get()->ExcuteParamEvent("CallPosToCoord", &pack);
	pair<int, int> coordD = pack.second;

	int dist = abs(coordO.first - coordD.first) + abs(coordO.second - coordD.second);

	//����ڰ� ������ ��Ÿ� �� => ���ʿ� ��Ÿ� ���̶� ���� ��ü�� ���� �� ��
	if (offenseCharacter->GetAttackRange().first > dist || offenseCharacter->GetAttackRange().second < dist)
		return;

	//�������ͽ��� ���� �̸� ������ ����
	attacks.push({ CalcDamage(offenseCharacter, defense), offense, defense });		//������ ����
	
	//����ڰ� ĳ���Ϳ��� ��� => �ݰ�
	Character* defenseCharacter = dynamic_cast<Character*>(defense);
	//�����ڰ� ����� ��Ÿ� �ȿ� ���� ������ �ݰ��� ����
	if (defenseCharacter != nullptr) {
		if (defenseCharacter->GetAttackRange().first <= dist && defenseCharacter->GetAttackRange().second >= dist)
			attacks.push({ CalcDamage(defenseCharacter, offense), defenseCharacter, offense });	//����� �ݰ�
	}

	//������ �����ߴ�
	isBattle = !attacks.empty();
	curOffense = offense;
	curDefense = defense;

	//ī�޶� ����
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

void CharacterManager::SetCharacterShader(wstring shader)
{
	for (auto& instance : instances)
		instance.second->SetShader(shader);
}

void CharacterManager::SetObjectShader(wstring shader)
{
	for (auto object : objectPool[SRPGObject::NONE]) {
		auto obstacle = (Obstacle*)object;
		obstacle->SetShader(shader);
	}
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

	//���� ���� �˸�
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
	//���� = �� + ���� ���� * (��ȿ ? 2 : 1) 
	int attack = attacker->CalcAttack();
	int defence = defender->GetStatus().defence;

	//���ط� = (���� - ����)*(1 or 3(ũ��Ƽ��) )
	int damage = attack - defence;

	return damage;
}
