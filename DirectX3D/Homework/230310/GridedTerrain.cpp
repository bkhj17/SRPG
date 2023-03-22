#include "framework.h"
#include "GridedTerrain.h"
#include "Character.h"

GridedTerrain::GridedTerrain()
{
	material->SetShader(L"LandScape/Terrain.hlsl");

	widthHeightBuffer = new FloatValueBuffer();
	widthHeightBuffer->Get()[0] = (float)width;
	widthHeightBuffer->Get()[1] = (float)height;
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
	for (UINT i = 0; i < row; i++) {
		for (UINT j = 0; j < col; j++) {
			size_t index = (size_t)i * col + j;
			cubes[index] = new Cube(Vector3(tileWidth, 3.0f, (float)tileHeight));
			cubes[index]->GetMaterial()->SetShader(L"Tile/Tile.hlsl");

			Vector3 pos((float)tileWidth * j + (tileWidth >> 1), 0, (float)tileHeight * (row-i) + (tileHeight >> 1));
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

}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;

	for (auto cube : cubes)
		delete cube;
	cubes.clear();
	cubes.shrink_to_fit();

	delete tileColorBuffer;

	objects.clear();
	movables.clear();
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
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();

	int index = 0;
	for (int i = 0; i < cubes.size(); i++) {
		if(!cubes[i]->Active())
			continue;

		if (movables.find(i) != movables.end())
			tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
		else
			tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		tileColorBuffer->SetPS(9);

		cubes[i]->Render();
	}

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

void GridedTerrain::SetSelected(int w, int h)
{
	int s = CoordToIndex(w, h);
	if (s != selected) {
		selected = s;

		if(!hold)
			CheckMovableArea();
	}
}

void GridedTerrain::InputAction(int w, int h)
{
	if (!hold) {
		//오브젝트 중 유효한 것이 있는지 확인
		for (auto object : objects) {
			Character* character = (Character*)object;
			if (character == nullptr)
				continue;

			pair<int, int> coord = PosToCoord(character->GlobalPos());
			if (coord.first != w || coord.second != h)
				continue;

			//사실 턴 플레이어인지도 알아야 한다
			//나중에 싱글톤으로 턴 정보 만들어야 함
			hold = true;
		}
	}
	else {

		//생각보다 자주 쓴다
		//매니저를 만들어야 하나?
		Character* selectedCharacter = nullptr;
		for (auto object : objects) {
			Character* character = (Character*)object;
			if (character != nullptr) {
				selectedCharacter = character;
				break;
			}
		}

		if (!selectedCharacter) {
			//캐릭터가 없는데도 홀드된 상태 : 무효 처리
			hold = false;
			CheckMovableArea();
			return;
		}

		//test : 이동을 선택한 것으로 가정
		//현재 위치(w, h)가 이동 가능한 위치라면
		//경로 역추적해서
		//character에 경로 째로 넘겨줘야 한다

		//경로 역추적. 이걸 위해 movable.value.second를 이전 노드로 설정했다

		vector<Vector3> path;
		int index = CoordToIndex(w, h);
		//갈 수 있는 위치가 아닌데 그냥 커서만 올라가 있는 거
		if (movables.find(index) == movables.end())
			return;

		if (movables[index].second == -1) {
			//캐릭터 위치 그 자체
			hold = false;
		}

		while (index != -1) {
			pair<int, int> pathCoord = IndexToCoord(index);
			path.push_back(CoordToPos(pathCoord.first, pathCoord.second));
			index = movables[index].second;
		}
		//맨 뒤 == 현 위치
		path.pop_back();
		
		selectedCharacter->SetMovePath(path);
	}
}

void GridedTerrain::CheckMovableArea()
{
	int range = 0;

	Character* character = nullptr;
	for (auto object : objects) {
		auto coord = PosToCoord(object->GlobalPos());
		int index = CoordToIndex(coord.first, coord.second);

		if (index == selected) {
			character = (Character*)object;
			if (character)
				range = character->GetMaxMove();
			break;
		}
	}

	movables.clear();

	const int dx[] = { 0,0,1,-1 };
	const int dy[] = { 1,-1,0,0 };

	pair<int, int> coord = IndexToCoord(selected);
	priority_queue<pair<int, int>> pq;
	pq.push({ 0, selected });
	movables[selected] = { 0, -1 };

	while (!pq.empty()) {
		int dist = pq.top().first;
		int cur = pq.top().second;
		pq.pop();

		pair<int, int> coord = IndexToCoord(cur);

		int nDist = dist + 1;
		if (nDist > range)
			continue;

		for (int i = 0; i < 4; i++) {
			int nx = coord.first + dx[i];
			int ny = coord.second + dy[i];

			if (nx < 0 || nx >= (int)col || ny < 0 || ny >= (int)row)
				continue;

			int nIndex = CoordToIndex(nx, ny);

			//유효하지 않은 타일
			if (!cubes[nIndex]->Active())
				continue;

			//


			if (movables.find(nIndex) != movables.end() && movables[nIndex].first <= nDist)
				continue;

			movables[nIndex] = { nDist, cur };
			pq.push({ nDist, CoordToIndex(nx, ny) });
		}
	}
}
