#pragma once

class GridedTerrain : public Terrain
{
public:
	enum SelectAction {
		MOVE, ATTACK,
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

	void InputAction(int w, int h);

	void CheckMovableArea();
	void CheckAttackableArea();

	UINT Row() { return row; }
	UINT Col() { return col; }

	Transform* ObjectOnIndex(int index);

private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;

	UINT tileWidth = 10, tileHeight = 10;
	UINT row, col;

	int selected = -1;

	ColorBuffer* tileColorBuffer;

	//필드 위 오브젝트는 등록 방식으로 
	vector<Transform*> objects;
	unordered_map<int, pair<int, int>> movables;
};

