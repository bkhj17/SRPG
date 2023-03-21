#pragma once
class GridedTerrain;
class MapCursor : public Transform
{
public:
	MapCursor();
	~MapCursor();

	void Update();
	void Render();

	int& GetW() { return w; }
	int& GetH() { return h; }

	void SetMapGrid(int row, int col) { this->row = row; this->col = col; }
	bool IsMoving() { return isMoving; }

	void SetTargetPos();

	void SetGridTerrain(GridedTerrain* terrain) { this->terrain = terrain; }
private:
	int row = 10, col = 10;
	int w = 5, h = 5;
	bool isMoving = false;


	SphereCollider* object;

	GridedTerrain* terrain = nullptr;
};

