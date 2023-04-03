#pragma once

class TurnManager : public Singleton<TurnManager>
{
private:
	friend class Singleton;
	TurnManager();
	~TurnManager();
public:
	void Init() { curTurn = 1; curPlayer = 1; }
	
	void Control(void* pack = nullptr);

	void NextTurn();

	int GetCurTurn() { return curTurn; }
	int GetCurPlayer() { return curPlayer; }
private:
	int curTurn = 1;
	int curPlayer = 0;
	vector<class Controller*> controllers;
};

