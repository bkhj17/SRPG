#include "framework.h"
#include "MainSelectUI.h"

MainSelectUI::MainSelectUI(Vector3 pos)
	: SelectUI({ 200.0f, 100.0f }, pos)
{
	text = { "턴 종료", "취소" };
	maxCursor = text.size();
}

void MainSelectUI::ActiveFunc()
{
	switch (cursor)
	{
	case 0:
		TurnManager::Get()->NextTurn();
		Close();
		break;
	default:
		CancelFunc();
		break;
	}
}