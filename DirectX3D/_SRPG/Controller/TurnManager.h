#pragma once

class TurnManager : public Singleton<TurnManager>
{
private:
	friend class Singleton;
	TurnManager();
	~TurnManager();
public:
	void Control();


	void NextTurn();

	int GetCurTurn() { return curTurn; }
	int GetCurPlayer() { return curPlayer; }
private:
	int curTurn = 0;
	int curPlayer = 0;
	vector<class Controller*> controllers;
};

