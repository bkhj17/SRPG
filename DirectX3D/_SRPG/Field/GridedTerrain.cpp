#include "framework.h"
#include "GridedTerrain.h"
#include "../Character/Character.h"
#include "../Character/CharacterManager.h"

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
			cubes[index] = new Cube(Vector3((float)tileWidth, 3.0f, (float)tileHeight));
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


	Observer::Get()->AddEvent("BattleEnd", bind(&GridedTerrain::CheckMovableArea, this));
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
	//���콺 ��� �ڵ�. Ȥ�� �𸣴� ���ܵ�
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
		else if(attackables.find(i) != attackables.end())
			tileColorBuffer->Get() = Float4(1.0f, 0.0f, 0.0f, 0.7f);
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

int GridedTerrain::CoordToIndex(pair<int, int> coord)
{
	return CoordToIndex(coord.first, coord.second);
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
	for (auto o : objects) {
		if (o == object)
			return;
	}

	objects.push_back(object);
}

void GridedTerrain::SetSelected(int w, int h)
{
	int s = CoordToIndex(w, h);
	if (s != selected) {
		selected = s;

		if(CharacterManager::Get()->HoldedCharacter() == nullptr)
			CheckMovableArea();
	}
}

void GridedTerrain::InputAction(int w, int h, SelectAction selectAction)
{
	//���� ���õ� ĳ���Ͱ� ����
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		selectAction = SELECT;
	
	switch (selectAction)
	{
	case GridedTerrain::SELECT:	
		SelectCharacter(w, h);
		break;
	case GridedTerrain::MOVE:
		SelectMove(w, h);
		break;
	case GridedTerrain::ATTACK:
		SelectAttack(w, h);
		break;
	default:
		break;
	}	
}

void GridedTerrain::CheckMovableArea()
{
	//������Ʈ���� ��ǥ Ž��
	map<int, Transform*> objectsOnIndex;
	for (auto object : objects) {
		auto coord = PosToCoord(object->GlobalPos());
		int index = CoordToIndex(coord.first, coord.second);
		objectsOnIndex[index] = object;
	}

	//���� ��ġ�� ������Ʈ�� �ִٸ� �ִ� �̵� �Ÿ� ���ϱ�
	int moveRange = 0;
	pair<int, int> attackRange = { 0, 0 };
	if (objectsOnIndex.find(selected) != objectsOnIndex.end()) {
		Character* character = (Character*)objectsOnIndex[selected];
		if (character) {
			moveRange = character->GetMaxMove();
			attackRange = character->GetAttackRange();
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
		if (nDist > moveRange)
			continue;

		for (int i = 0; i < 4; i++) {
			int nx = coord.first + dx[i];
			int ny = coord.second + dy[i];

			if (nx < 0 || nx >= (int)col || ny < 0 || ny >= (int)row)
				continue;

			int nIndex = CoordToIndex(nx, ny);

			//��ȿ���� ���� Ÿ��
			if (!cubes[nIndex]->Active())
				continue;

			//�ش��ϴ� ��ġ�� �ٸ� ������Ʈ�� �ִ�
			if (objectsOnIndex.find(nIndex) != objectsOnIndex.end()) {
				//���� �� �����̸� ������ ���� �ִ�.
				//Ŀ������ ĭ ���� �� ������Ʈ ������ �Ǻ��ؾ߰ڳ�
				continue;
			}
			//�̹� Ž���� Ÿ���̴�
			if (movables.find(nIndex) != movables.end() && movables[nIndex].first <= nDist)
				continue;

			movables[nIndex] = { nDist, cur };
			pq.push({ nDist, CoordToIndex(nx, ny) });
		}
	}

	CheckAttackableArea(attackRange.first, attackRange.second);
}

void GridedTerrain::CheckAttackableArea(int minRange, int maxRange, bool isStand)
{
	//���ݹ��� ����
	//movable

	vector<int> checkIndices;

	attackables.clear();
	if (isStand) {
		checkIndices.push_back(selected);
	}
	else {
		for (auto& movable : movables)
			checkIndices.push_back(movable.first);
	}

	pair<int, int> coord;
	for (int index : checkIndices) {		
		coord = IndexToCoord(index);
		for (int y = -maxRange; y <= maxRange; y++) {
			for (int x = -(maxRange - abs(y)); x <= maxRange - abs(y); x++) {
				if (abs(y) + abs(x) < minRange)
					continue;

				int nx = coord.first + x;
				int ny = coord.second + y;
				if (nx < 0 || nx >= (int)col || ny < 0 || ny >= (int)row)
					continue;

				int nIndex = CoordToIndex(nx, ny);
				if (!cubes[nIndex]->Active())
					continue;

				attackables[nIndex] = true;
			}
		}
	}
}

Transform* GridedTerrain::ObjectOnIndex(int index)
{
	for (auto object : objects) {
		int oi = CoordToIndex(PosToCoord(object->Pos()));
		if (oi == index)
			return object;
	}

	return nullptr;
}

void GridedTerrain::SelectCharacter(int w, int h)
{
	//�ش� ��ġ�� ������Ʈ �� ��ȿ�� ���� �ִ��� Ȯ��
	for (auto object : objects) {
		Character* character = (Character*)object;
		if (character == nullptr)
			continue;

		pair<int, int> coord = PosToCoord(character->GlobalPos());
		if (coord.first != w || coord.second != h)
			continue;

		//��� �� �÷��̾������� �˾ƾ� �Ѵ�
		//���߿� �̱������� �� ���� ������ ��
		if (!character->IsActed()) {
			CharacterManager::Get()->CharacterHold(character, w, h);
			break;
		}
	}

	//��ġ���� ���õ� ���� ����
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		CheckMovableArea();
}

void GridedTerrain::SelectMove(int w, int h)
{
	//�̵� ����
	
	//�̹� �˻��ϰ� ������� ������ �׷��� �ϴ� ����ó��
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		return;

	//Ŀ�� ��ġ
	int index = CoordToIndex(w, h);
	//�� �� �ִ� ��ġ�� �ƴѵ� �׳� Ŀ���� �ö� �ִ� ��
	if (movables.find(index) == movables.end())
		return;

	if (movables[index].second == -1) {
		//ĳ���� ��ġ �� ��ü == ��������
		CharacterManager::Get()->CharacterUnhold();
		return;
	}

	//���� ��ġ(w, h)�� �̵� ������ ��ġ
	vector<Vector3> path;
	while (index != -1) {
		pair<int, int> pathCoord = IndexToCoord(index);
		path.push_back(CoordToPos(pathCoord.first, pathCoord.second));
		//��� ������. �̰� ���� movable.value.second�� ���� ���� �����ߴ�
		index = movables[index].second;
	}
	//�� �� == �� ��ġ
	path.pop_back();

	//character�� ��� °�� �Ѱ���� �Ѵ�
	CharacterManager::Get()->HoldedCharacter()->SetMovePath(path);
}

void GridedTerrain::SelectAttack(int w, int h)
{
	//���� ����

	//�̹� �˻��ϰ� ������� ������ �׷��� �ϴ� ����ó��
	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded == nullptr)
		return;

	auto holdedPos = PosToCoord(holded->Pos());
	auto range = holded->GetAttackRange();

	//Ŀ�� ��ġ�� ��Ÿ� ���̸� �н�
	int dist = abs(holdedPos.first - w) + abs(holdedPos.second - h);
	if (range.first > dist || range.second < dist)
		return;
	
	//Ŀ�� ��ġ�� ĳ���Ͱ� �ֱ�� ���� Ȯ��
	Character* attacked = nullptr;
	for (auto object : objects) {
		Character* character = (Character*)object;
		//ĳ���Ͱ� �ƴϰų� ��Ȱ�� ���¸� �н�
		if (character == nullptr || !character->Active())
			continue;

		if (character == holded)
			continue;

		auto coord = PosToCoord(character->Pos());
		if (coord.first == w && coord.second == h) {
			attacked = character;
			break;
		}
	}

	if (attacked == nullptr)
		return;

	//������ ��Ʋ ���� - ��¥ ���������� �Ŵ������� �Ǵ��ϵ��� ����
	CharacterManager::Get()->BattleStart(holded, attacked);
}

bool GridedTerrain::IsActiveCoord(int w, int h)
{
	if (w < 0 || w >= col || h < 0 || h >= row)
		return false;

	return cubes[CoordToIndex(w, h)]->Active();
}