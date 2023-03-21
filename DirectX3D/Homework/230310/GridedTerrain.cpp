#include "framework.h"
#include "GridedTerrain.h"
#include "MapCursor.h"

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

	cursor = new MapCursor;
	cursor->SetGridTerrain(this);
	cursor->SetMapGrid(row, col);
	cursor->GetW() = 5;
	cursor->GetH() = 5;
}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;

	for (auto cube : cubes)
		delete cube;
	cubes.clear();
	cubes.shrink_to_fit();

	delete tileColorBuffer;

	delete cursor;

	objects.clear();
	selectables.clear();
}

void GridedTerrain::Update()
{
	/*
	//마우스 기반 코드. 혹시 모르니 남겨둠
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

	cursor->Update();
	int s = CoordToIndex(cursor->GetW(), cursor->GetH());
	if (s != selected) {
		selected = s;
		Test();
	}
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();

	int index = 0;
	for (int i = 0; i < cubes.size(); i++) {
		if(!cubes[i]->Active())
			continue;

		if (selectables.find(i) != selectables.end())
			tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
		else
			tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		tileColorBuffer->SetPS(9);

		cubes[i]->Render();
	}

	cursor->Render();
}

int GridedTerrain::CoordToIndex(int x, int y)
{
	return y * col + x;
}

Vector3 GridedTerrain::CoordToPos(int x, int y)
{
	Vector3 pos = cubes[(size_t)y * col + x]->GlobalPos();
	pos.y += cubes[(size_t)y * col + x]->GlobalScale().y;
	return pos;
}

pair<int, int> GridedTerrain::IndexToCoord(int index)
{
	return {index % col, index / col};
}

pair<int, int> GridedTerrain::PosToCoord(Vector3 pos)
{
	int x = (int)pos.x / tileWidth;
	int y = row - (int)pos.z / tileHeight;

	return { x, y };
}

void GridedTerrain::AddObject(Transform* object)
{
	objects.push_back(object);
}

void GridedTerrain::Test()
{
	int range = 0;

	for (auto object : objects) {
		auto coord = PosToCoord(object->GlobalPos());
		int index = CoordToIndex(coord.first, coord.second);

		if (index == selected) {
			range = 2;
			break;
		}
	}

	selectables.clear();

	const int dx[] = { 0,0,1,-1 };
	const int dy[] = { 1,-1,0,0 };

	pair<int, int> coord = IndexToCoord(selected);
	priority_queue<pair<int, int>> pq;
	pq.push({ 0, selected });
	while (!pq.empty()) {
		int dist = pq.top().first;
		int cur = pq.top().second;
		pq.pop();

		if (dist > range)
			continue;

		if (selectables.find(cur) != selectables.end() && selectables[cur] <= dist)
			continue;

		selectables[cur] = dist;

		pair<int, int> coord = IndexToCoord(cur);

		int nDist = dist + 1;
		for (int i = 0; i < 4; i++) {
			int nx = coord.first + dx[i];
			int ny = coord.second + dy[i];

			if (nx < 0 || nx >= col || ny < 0 || ny >= row)
				continue;

			pq.push({ nDist, CoordToIndex(nx, ny) });
		}
	}
}
