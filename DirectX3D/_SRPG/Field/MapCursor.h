#pragma once
class GridedTerrain;
class MapCursor : public Transform
{
private:
	const float CAM_MOVE_VALUE = 0.1f;
	const float MOVE_SPEED = 20.0f;
	const float ON_OBJECT_Y = 12.0f;
public:
	MapCursor();
	~MapCursor();

	void Control();
	void Update();
	virtual void UpdateWorld() override;
	void Render();

	int& GetW() { return w; }
	int& GetH() { return h; }

	void SetPosCoord(int w, int h, bool teleport = false);
	void SetMapGrid(int row, int col) { this->row = row; this->col = col; }
	bool IsMoving() { return isMoving; }

	void SetGridTerrain(GridedTerrain* terrain);

	void FocusPos(void* posPtr);
private:
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

