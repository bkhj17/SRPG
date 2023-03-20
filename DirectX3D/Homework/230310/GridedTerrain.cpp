#include "framework.h"
#include "GridedTerrain.h"

GridedTerrain::GridedTerrain()
{
	material->SetShader(L"LandScape/Terrain.hlsl");

	widthHeightBuffer = new FloatValueBuffer();
	widthHeightBuffer->Get()[0] = width;
	widthHeightBuffer->Get()[1] = height;
	widthHeightBuffer->Get()[2] = (float)tileWidth;
	widthHeightBuffer->Get()[3] = (float)tileHeight;

	row = width / tileWidth;
	col = height / tileHeight;

	Vector3 up = Vector3(0, 1, 0);

	float halfX = tileWidth * 0.5f;
	float halfZ = tileHeight * 0.5f;
	vector<Vector3> v = {
		Vector3(-halfX, 0, -halfZ),
		Vector3(halfX, 0, -halfZ),
		Vector3(-halfX, 0, halfZ),
		Vector3(halfX, 0, halfZ)
	};

	vector<VertexType>& vertices = mesh->GetVertices();
	cubes.resize((size_t)row*col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			size_t index = (size_t)i * col + j;
			cubes[index] = new Cube(Vector3(tileWidth, 3.0f, tileHeight));
			cubes[index]->GetMaterial()->SetShader(L"Tile/Tile.hlsl");

			Vector3 pos(tileWidth * j + (tileWidth >> 1), 0, tileHeight * (row-i) + (tileHeight >> 1));
			cubes[index]->Pos() = pos;
			cubes[index]->Pos().y = GetHeight(pos);
			cubes[index]->UpdateWorld();

			bool check = cubes[index]->Pos().y < MAX_HEIGHT * 0.1f;
			if (check) {
				for (const auto& dv : v) {
					if (GetHeight(dv + pos) > MAX_HEIGHT * 0.1f) {
						check = false;
						break;
					}
				}
			}
			cubes[index]->SetActive(check);
		}
	}

	tileColorBuffer = new ColorBuffer;

	w = 5;
	h = 5;
	testCursor = new SphereCollider;
	testCursor->Pos() = CoordToPos(w, h);
	testCursor->UpdateWorld();
}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;

	for (auto cube : cubes)
		delete cube;
	cubes.clear();
	cubes.shrink_to_fit();

	delete tileColorBuffer;

	delete testCursor;
}

void GridedTerrain::Update()
{
	/*
	Ray ray = CAM->ScreenPointToRay(mousePos);
	float dist = FLT_MAX;

	for (int i = 0; i < cubes.size(); i++) {
		if (!cubes[i]->Active())
			continue;

		Contact contact;
		if (cubes[i]->GetCollider()->IsRayCollision(ray, &contact)) {
			if (contact.distance < dist) {
				dist = contact.distance;
				selected = i;
			}
		}
	}
	*/
	UpdateWorld();

	if (!isMoving) {

		if (KEY_PRESS(VK_UP)) {
			if (h < row - 1)
				h--;
		}
		if (KEY_PRESS(VK_DOWN)) {
			if (h > 0)
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
	selected = h * col + w;
	Vector3 targetPos = CoordToPos(w, h);
	targetPos.y = 3.0f;

	isMoving = ((targetPos - testCursor->Pos()).Length() > 1.0f);
	if (isMoving)
		testCursor->Pos() = Lerp(testCursor->Pos(), targetPos, 20.0f * DELTA);
	else
		testCursor->Pos() = targetPos;
	
	testCursor->UpdateWorld();
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();

	int index = 0;
	for (int i = 0; i < cubes.size(); i++) {
		if(!cubes[i]->Active())
			continue;

		if (i == selected)
			tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
		else
			tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		tileColorBuffer->SetPS(9);

		cubes[i]->Render();
	}

	testCursor->Render();
}

Vector3 GridedTerrain::CoordToPos(int x, int y)
{
	return cubes[(size_t)y * col + x]->Pos();
}
