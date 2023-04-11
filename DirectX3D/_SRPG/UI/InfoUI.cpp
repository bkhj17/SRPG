#include "framework.h"

InfoUI::InfoUI(Vector3 pos)
	: UIWindow(Vector2(200.0f, 150.0f), pos)
{
	isActive = true;
}

void InfoUI::Render()
{
	if (!Active() || object == nullptr)
		return;
	__super::Render();

	float x = Pos().x - Half().x + 20.0f;
	float y = Pos().y + Half().y - 25.0f;

	//이름
	Font::Get()->RenderTextLeft(object->GetStatus().name, { x, y });
	//팀
	wstring team = object->GetStatus().teamNum == Character::Team::PLAYER ? L"Player" : L"Enemy";
	Font::Get()->RenderText(team, { Pos().x + Half().x - 20.0f, y});
	
	//hp
	wstring hpText = L"HP : " + to_wstring(object->GetStatus().curHp) + L"/" + to_wstring(object->GetStatus().maxHp);
	Font::Get()->RenderTextLeft(hpText, {x , y - 30.0f});

	//캐릭터일 경우 무기 종류
	Character* character = dynamic_cast<Character*>(object);
	if (character) {
		//이동거리
		wstring moveText = L"이동 : " + to_wstring(object->GetStatus().move);
		Font::Get()->RenderTextLeft(moveText, { x,  y - 30.0f * 2.0f });

		Weapon* weapon = character->GetWeapon();
		if (weapon) {
			//무기 이름
			string weaponName = WeaponManager::Get()->GetTypeName(weapon->GetType());
			Font::Get()->RenderTextLeft(weaponName, { x, y - 30.0f * 3.0f });

			//무기 사거리
			auto range = weapon->GetRange();
			wstring rangeText = to_wstring(range.first) + L"~" + to_wstring(range.second);
			Font::Get()->RenderTextLeft(rangeText, { x + Half().x,  y - 30.0f * 3.0f });
		}
	}
}

void InfoUI::SetObject(SRPGObject* object)
{
	if (object != nullptr && !object->Active()) {
		this->object = nullptr;
		return;
	}

	this->object = object;
}
