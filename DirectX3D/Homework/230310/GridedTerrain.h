#pragma once

class GridedTerrain : public Terrain
{
public:
	GridedTerrain();
	~GridedTerrain();

	void Update();
	void Render();


	int CoordToIndex(int x, int y);
	Vector3 CoordToPos(int x, int y);
	pair<int, int> IndexToCoord(int index);
	pair<int, int> PosToCoord(Vector3 pos);

	void AddObject(Transform* object);
private:
	void Test();

private:
	FloatValueBuffer* widthHeightBuffer;

	vector<Cube*> cubes;

	UINT tileWidth = 10, tileHeight = 10;
	UINT row, col;

	int selected;
	ColorBuffer* tileColorBuffer;

	class MapCursor* cursor;

	//필드 위 오브젝트는 등록 방식으로 
	vector<Transform*> objects;
	unordered_map<int, int> selectables;
};

