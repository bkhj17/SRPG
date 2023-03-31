#include "framework.h"

ActionSelectUI::ActionSelectUI(Vector3 pos)
	: UIWindow({ 200.0f, 200.0f }, pos)
{
	renderCursor = true;
	maxCursor = text.size();
}

void ActionSelectUI::Update()
{
	if (!Active())
		return;

	__super::Update();

	cQuad->Pos().x = CursorX();
	cQuad->Pos().y = LineY(cursor);
	cQuad->UpdateWorld();
}

void ActionSelectUI::Render()
{
	if (!Active())
		return;

	__super::Render();
	for(int i = 0; i < text.size(); i++)
		Font::Get()->RenderText(text[i], Vector2(GlobalPos().x, GlobalPos().y + LineY(i)));
}

void ActionSelectUI::Close()
{
	__super::Close();
	CharacterManager::Get()->CharacterUnhold();
}

void ActionSelectUI::ActiveFunc()
{
	switch (cursor)
	{
	case 0:
		SRPGUIManager::Get()->OpenUI("MapSelectMove");
		break;
	case 1:
		SRPGUIManager::Get()->OpenUI("MapSelectAttack");
		break;
	case 2:
		if(CharacterManager::Get()->HoldedCharacter() != nullptr)
			CharacterManager::Get()->HoldedCharacter()->ActEnd();
		Observer::Get()->ExcuteEvent("CheckMovable");
		Close();
		break;
	default:
		CancelFunc();
		break;
	}
}

void ActionSelectUI::CancelFunc()
{
	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded != nullptr && holded->IsMoved()) {
		CharacterManager::Get()->HoldedCharacter()->CancelMove();
		SRPGUIManager::Get()->OpenUI("MapSelectMove");
	}
	else {
		Close();
	}
}

float ActionSelectUI::CursorX()
{
	return -Half().x + 30.0f;
}

float ActionSelectUI::LineY(int lineNum)
{
	return Half().y - 40.0f - lineNum * (cQuad->Half().y + 30.0f);
}
