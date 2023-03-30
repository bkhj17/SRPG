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

	void SetPosCoord(int w, int h, bool teleport = false);
	void SetMapGrid(int row, int col) { this->row = row; this->col = col; }
	bool IsMoving() { return isMoving; }

	void SetGridTerrain(GridedTerrain* terrain);

	void FocusPos(void* posPtr);
private:
	void Control();
	void Move();

	void CamMove();
private:
	int row = 10, col = 10;
	int w = 5, h = 5;

	bool isMoved = false;
	bool isMoving = false;

	Vector3 objectPos = { 0,0,0 };
	SphereCollider* object;

	GridedTerrain* terrain = nullptr;
};

