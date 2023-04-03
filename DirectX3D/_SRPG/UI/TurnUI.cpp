#include "framework.h"
#include "TurnUI.h"

TurnUI::TurnUI(Vector3 pos)
	: UIWindow(Vector2(300.0f, 60.0f), pos)
{
	isActive = true;
}

void TurnUI::Render()
{
	__super::Render();

	wstring turnPlayer = TurnManager::Get()->GetCurPlayer() == 0 ? L"Player" : L"Enemy";
	Font::Get()->RenderTextLeft(turnPlayer, Vector2(Pos().x - Half().x + 30.0f, Pos().y));

	wstring turn = L"Turn " + to_wstring(TurnManager::Get()->GetCurTurn());
	Font::Get()->RenderText(turn, Vector2(Pos().x + 100.0f, Pos().y));
}
