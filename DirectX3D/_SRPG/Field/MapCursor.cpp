#include "framework.h"

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

#if DEBUG_MODE
	if (KEY_DOWN('9')) {
		camMoveOn = !camMoveOn;
	}
#endif
}

void MapCursor::UpdateWorld()
{
	__super::UpdateWorld();

	//Ŀ�� �ö� ĭ�� ������Ʈ�� �ִٸ� Ŀ�� ������Ʈ ���� �ø���
	SRPGObject* inPoint = terrain->ObjectOnIndex(terrain->CoordToIndex({ w, h }));
	if (inPoint)
		objectPos.y = inPoint->GetCursorHeight();

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
#if DEBUG_MODE
	if (!camMoveOn)
		return;
#endif // DEBUG_MODE


	Vector3 camMove = {};
	Vector3 cursorScreen = CAM->WorldToScreen(GlobalPos());

	if (cursorScreen.x <= WIN_WIDTH * CAM_MOVE_VALUE)
		camMove -= CAM->Right();
	if (cursorScreen.x >= WIN_WIDTH * (1 - CAM_MOVE_VALUE))
		camMove += CAM->Right();
	if (cursorScreen.y <= WIN_HEIGHT * CAM_MOVE_VALUE)
		camMove -= CAM->Up();
	if (cursorScreen.y >= WIN_HEIGHT * (1 - CAM_MOVE_VALUE))
		camMove += CAM->Up();
	camMove.y = 0.0f;
	CAM->Pos() += camMove * 3.0f * MOVE_SPEED * DELTA;
}
