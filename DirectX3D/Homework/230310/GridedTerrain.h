#pragma once

class GridedTerrain : public Terrain
{
public:
	enum SelectAction {
		SELECT, MOVE, ATTACK,
	};

public:
	GridedTerrain();
	~GridedTerrain();

	void Update();
	void Render();

	int CoordToIndex(int x, int y);
	int CoordToIndex(pair<int, int> coord);

	Vector3 CoordToPos(int x, int y);
	pair<int, int> IndexToCoord(int index);
	pair<int, int> PosToCoord(Vector3 pos);

	void AddObject(Transform* object);

	void SetSelected(int w, int h);
	int GetSelected() { return selected; }

	void InputAction(int w, int h, SelectAction selectAction = MOVE);

	void CheckMovableArea();
	void CheckAttackableArea(int minRange, int maxRange, bool isStand = false);

	UINT Row() { return row; }
	UINT Col() { return col; }

	Transform* ObjectOnIndex(int index);
private:
	void SelectCharacter(int w, int h);
	void SelectMove(int w, int h);
	void SelectAttack(int w, int h);
private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;

	UINT tileWidth = 10, tileHeight = 10;
	UINT row, col;

	int selected = -1;

	ColorBuffer* tileColorBuffer;

	//필드 위 오브젝트는 등록하는 방식으로 
	vector<Transform*> objects;
	
	//행동 범위 저장 : 이동, 공격
	unordered_map<int, pair<int, int>> movables;
	unordered_map<int, bool> attackables;
};

