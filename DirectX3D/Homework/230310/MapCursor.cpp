#include "framework.h"
#include "MapCursor.h"
#include "GridedTerrain.h"

MapCursor::MapCursor()
{
	object = new SphereCollider;
}

MapCursor::~MapCursor()
{
	delete object;
}

void MapCursor::Update()
{
	Control();
	Move();
}

void MapCursor::Render()
{
	object->Render();
}

void MapCursor::SetPosCoord(int w, int h, bool teleport)
{
	if (!terrain)
		return;

	Pos() = terrain->CoordToPos(w, h);
	Pos().y += 3.0f;

	if (teleport)
		object->Pos() = Pos();
}

void MapCursor::SetGridTerrain(GridedTerrain* terrain)
{
	this->terrain = terrain;
	SetMapGrid(terrain->Row(), terrain->Col());


	terrain->SetSelected(w, h);
}

void MapCursor::Control()
{
	if (!isMoving) {
		if (KEY_PRESS(VK_UP)) {
			if (h > 0)
				h--;
		}
		if (KEY_PRESS(VK_DOWN)) {
			if (h < row - 1)
				h++;
		}
		if (KEY_PRESS(VK_LEFT)) {
			if (w > 0)
				w--;
		}
		if (KEY_PRESS(VK_RIGHT)) {
			if (w < col - 1)
				w++;
		}
	}
}

void MapCursor::Move()
{
	if (!isMoving && terrain) {
		SetPosCoord(w, h);
		terrain->SetSelected(w, h);
	}
	UpdateWorld();

	isMoving = ((GlobalPos() - object->Pos()).Length() > 1.0f);
	if (isMoving)
		object->Pos() = Lerp(object->Pos(), GlobalPos(), 20.0f * DELTA);
	else
		object->Pos() = GlobalPos();
	object->UpdateWorld();
}
