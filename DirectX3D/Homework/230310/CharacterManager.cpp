#include "framework.h"
#include "CharacterManager.h"
#include "Character.h"

CharacterManager::CharacterManager()
{
	characterPool.resize(MAX_POOL);
	for (auto& character : characterPool) {
		character = new Character();
		character->SetActive(false);
	}
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
}

void CharacterManager::Render()
{
	for (auto character : characterPool)
		character->Render();
}

void CharacterManager::CharacterUnhold()
{
	if (holded->moved)
		holded->ActEnd();
	
	CharacterHold(nullptr);
}

bool CharacterManager::IsActing()
{
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

void CharacterManager::BattleStart(Character* offense, Character* defense)
{
	if (!offense || !offense->Active()
		|| !defense || !defense->Active()) {

	}
}

void CharacterManager::BattleEnd()
{
}
