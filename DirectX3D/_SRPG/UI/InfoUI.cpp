#include "framework.h"

InfoUI::InfoUI(Vector3 pos)
	: UIWindow(Vector2(200.0f, 120.0f), pos)
{
	isActive = true;
}

void InfoUI::Render()
{
	if (!Active() || character == nullptr)
		return;
	__super::Render();

	float x = Pos().x - Half().x + 20.0f;
	float y = Pos().y + Half().y - 25.0f;

	wstring hpText = L"HP : " + to_wstring(character->GetStatus().curHp) + L"/" + to_wstring(character->GetStatus().maxHp);
	wstring moveText = L"ÀÌµ¿ : " + to_wstring(character->GetStatus().move);

	//ÀÌ¸§
	Font::Get()->RenderTextLeft(character->GetStatus().name, { x, y });
	//ÆÀ
	Font::Get()->RenderText(to_wstring(character->GetStatus().teamNum), { Pos().x + Half().x - 20.0f, y});
	
	//hp
	Font::Get()->RenderTextLeft(hpText, {x , y - 30.0f});

	Font::Get()->RenderTextLeft(moveText, {x,  y - 30.0f * 2.0f});
}

void InfoUI::SetCharacter(Character* character)
{
	if (character != nullptr && !character->Active()) {
		this->character = nullptr;
		return;
	}
	this->character = character;
}
