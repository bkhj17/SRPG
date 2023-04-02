#include "framework.h"
#include "MapCursor.h"
#include "GridedTerrain.h"

MapCursor::MapCursor()
{
	object = new SphereCollider;

	Observer::Get()->AddParamEvent("FocusPos", bind(&MapCursor::FocusPos, this, placeholders::_1));
}

MapCursor::~MapCursor()
{
	delete object;
}

void MapCursor::Control()
{
	if (!isMoving && !isMoved) {
		int nh = h;
		int nw = w;

		if (KEY_PRESS(VK_UP))		nh--;
		if (KEY_PRESS(VK_DOWN))		nh++;
		if (KEY_PRESS(VK_LEFT))		nw--;
		if (KEY_PRESS(VK_RIGHT))	nw++;

		if (terrain && terrain->IsActiveCoord(nw, nh)) {
			w = nw;
			h = nh;
		}
	}
}

void MapCursor::Update()
{
	Move();
	UpdateWorld();
}

void MapCursor::UpdateWorld()
{
	__super::UpdateWorld();

	//커서 올라간 칸에 오브젝트가 있다면 커서 오브젝트 높이 올리기
	Transform* inPoint = terrain->ObjectOnIndex(terrain->CoordToIndex({ w, h }));
	if (inPoint)
		objectPos.y = ON_OBJECT_Y;

	object->Pos() = (isMoving) ? Lerp(object->Pos(), objectPos, MOVE_SPEED * DELTA) : objectPos;
	object->UpdateWorld();

	CamMove();
}

void MapCursor::Render()
{
	object->Render();
}

void MapCursor::SetPosCoord(int w, int h, bool teleport)
{
	this->w = w;
	this->h = h;

	if (!terrain)
		return;

	Pos() = terrain->CoordToPos(w, h);
	Pos().y += 3.0f;
	UpdateWorld();
	objectPos = GlobalPos();

	if (teleport) {
		object->Pos() = objectPos;
		isMoving = true;
	}

	if (isMoved || teleport)
		terrain->SetSelected(w, h, teleport);
}

void MapCursor::SetGridTerrain(GridedTerrain* terrain)
{
	this->terrain = terrain;
	SetMapGrid(terrain->Row(), terrain->Col());

	terrain->SetSelected(w, h);
}

void MapCursor::FocusPos(void* posPtr)
{
	auto pos = (Vector3*)posPtr;

	auto coord = terrain->PosToCoord(*pos);
	SetPosCoord(coord.first, coord.second, true);
}

void MapCursor::Move()
{
	if (!isMoving)
		SetPosCoord(w, h);

	isMoved = isMoving;
	Vector3 vel = objectPos - object->Pos();
	vel.y = 0.0f;

	isMoving = vel.Length() > 1.0f;
}

void MapCursor::CamMove()
{
	Vector3 camMove = {};
	Vector3 cursorScreen = CAM->WorldToScreen(GlobalPos());

	if (cursorScreen.x <= WIN_WIDTH * CAM_MOVE_VALUE)
		camMove.x -= 1.0f;
	if (cursorScreen.x >= WIN_WIDTH * (1 - CAM_MOVE_VALUE))
		camMove.x += 1.0f;
	if (cursorScreen.y <= WIN_HEIGHT * CAM_MOVE_VALUE)
		camMove.z -= 1.0f;
	if (cursorScreen.y >= WIN_HEIGHT * (1 - CAM_MOVE_VALUE))
		camMove.z += 1.0f;
	CAM->Pos() += camMove * 3.0f * MOVE_SPEED * DELTA;
}
