#pragma once

class GridedTerrain : public Terrain
{
public:
	enum SelectAction {
		SELECT, MOVE, ATTACK,
	};

	struct MovableInfo {
		int dist = 1000000000;
		int prev = -1;
		bool standable = true;
	};

	const UINT DEFAULT_TILE_SIZE = 20;
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

	void SetSelected(int w, int h, bool forced = false);
	int GetSelected() { return selected; }

	void InputAction(int w, int h, SelectAction selectAction = MOVE);
	void InputAction(SelectAction selectAction = MOVE);

	void CheckMovableArea();
	void CheckAttackableArea(int minRange, int maxRange, bool isStand = false);

	UINT Row() { return row; }
	UINT Col() { return col; }

	Transform* ObjectOnIndex(int index);

	void SelectCharacter(int w, int h);
	void SelectMove(int w, int h);
	void SelectAttack(int w, int h);

	void StandingAttack(bool is) { standingAttack = is; }

	bool IsActiveCoord(int w, int h);
	Vector3 GetTileSize() { return Vector3((float)tileWidth, 0.0f, (float)tileHeight); }

	int GetSelectedIndex() { return selected; }
	vector<pair<class Character*, pair<int, int>>> AttackableCharacters(int targetTeam);

private:
	void CalledPosToCoord(void* pack);

private:
	void Reselect();
private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;

	UINT tileWidth = DEFAULT_TILE_SIZE, tileHeight = DEFAULT_TILE_SIZE;
	UINT row, col;

	int selected = -1;

	ColorBuffer* tileColorBuffer;

	//필드 위 오브젝트는 등록하는 방식으로 
	vector<Transform*> objects;
	
	//행동 범위 저장 : 이동, 공격
	unordered_map<int, MovableInfo> movables;
	unordered_map<int, pair<int, int>> attackables;

	bool standingAttack = false;
};

