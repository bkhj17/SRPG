#include "framework.h"
#include "TurnManager.h"

TurnManager::TurnManager()
{
	controllers.push_back(new PlayerController);
	//controllers.push_back(new EnemyController);
}

TurnManager::~TurnManager()
{
	for (auto controller : controllers)
		delete controller;
}

void TurnManager::Control()
{
	if (curPlayer >= controllers.size())
		return;

	controllers[curPlayer]->Control();
}

void TurnManager::NextTurn()
{
	if (++curPlayer >= controllers.size()) {
		curTurn++;
		curPlayer = 0;
	}

	Observer::Get()->ExcuteEvent("TurnStart");
}
