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
	this->w = w;
	this->h = h;

	if (!terrain)
		return;

	Pos() = terrain->CoordToPos(w, h);
	Pos().y += 3.0f;
	UpdateWorld();
	objectPos = GlobalPos();

	//커서 올라간 칸에 오브젝트가 있다면 커서 오브젝트 높이 올리기
	Transform* inPoint = terrain->ObjectOnIndex(terrain->CoordToIndex({ w, h }));
	if (inPoint)
		objectPos.y += 6.0f;

	if (teleport) {
		object->Pos() = objectPos;
		isMoving = true;
	}

	if (isMoved || teleport) {
		terrain->SetSelected(w, h, teleport);
		CamMove();
	}
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

void MapCursor::Control()
{
	if (!isMoving) {
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

void MapCursor::Move()
{
	if (!isMoving)
		SetPosCoord(w, h);

	isMoved = isMoving;
	isMoving = (objectPos - object->Pos()).Length() > 1.0f;
	
	object->Pos() = (isMoving) ? Lerp(object->Pos(), objectPos, 20.0f * DELTA) : objectPos;
	object->UpdateWorld();
}

void MapCursor::CamMove()
{
	//연속 시키기 힘듦. 원래 정확한 값을 연산해보려 했으나....
	//보간 없는 상황
	Vector3 cursorScreen = CAM->WorldToScreen(GlobalPos());
	Vector3 camMove;
	if (cursorScreen.x <= WIN_WIDTH * 0.1f)
		camMove.x = -terrain->GetTileSize().x;
	if (cursorScreen.x >= WIN_WIDTH * 0.9f)
		camMove.x = terrain->GetTileSize().x;
	if (cursorScreen.y <= WIN_HEIGHT * 0.1f)
		camMove.z = -terrain->GetTileSize().z;
	if (cursorScreen.y >= WIN_HEIGHT * 0.9f)
		camMove.z = terrain->GetTileSize().z;

	CAM->Pos() += camMove;
}
