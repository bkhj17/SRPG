#include "framework.h"
#include "Inventory230306.h"

Inventory230306::Inventory230306()
{
	inventory.resize(10);
	for (int i = 0; i < inventory.size(); i++) {
		inventory[i].first = new Quad(Vector2(30.0f, 30.0f));
		inventory[i].first->Pos() = startPos + posStep * (float)i;
		inventory[i].first->GetMaterial()->SetShader(L"SelectTexture230306.hlsl");
		inventory[i].first->UpdateWorld();
		inventory[i].second = 0;
	}

	selectBuffer = new IntValueBuffer;
}

Inventory230306::~Inventory230306()
{
	for (auto& i : inventory)
		delete i.first;
	inventory.clear();

	delete selectBuffer;
}

void Inventory230306::Input(Texture* texture)
{
	int p = -1;
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i].first->GetMaterial()->GetDiffuseMap() == texture) {
			inventory[i].second++;
			return;
		}
		else if (inventory[i].second == 0 && p == -1)
			p = i;
	}

	if (p != -1) {
		inventory[p].first->GetMaterial()->SetDiffuseMap(texture->GetFile());
		inventory[p].second = 1;
	}
}

Texture* Inventory230306::Select()
{
	if (select < 0 || select > inventory.size()) {
		return nullptr;
	}

	if (inventory[select].second == 0) {
		return nullptr;
	}
	
	return inventory[select].first->GetMaterial()->GetDiffuseMap();
}

void Inventory230306::OutputSelect()
{
	if (select < 0 || select > inventory.size())
		return;
	if (inventory[select].second == 0) 
		return;
	inventory[select].second--;
}

void Inventory230306::Update()
{
	for (int i = 0; i < 10; i++) {
		if (KEY_PRESS('0' + i + 1)) {
			select = i;
			break;
		}
	}
}

void Inventory230306::Render()
{
	for (int i = 0; i < inventory.size(); i++) {
		if (inventory[i].second != 0) {
			selectBuffer->Get()[0] = (int)(select == i);
			selectBuffer->SetPS(13);
			inventory[i].first->Render();

			Font::Get()->RenderText(
				to_string(inventory[i].second),
				Float2(inventory[i].first->Pos().x, inventory[i].first->Pos().y)
			);
		}
	}
}
