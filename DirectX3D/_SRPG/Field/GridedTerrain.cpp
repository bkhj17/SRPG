#include "framework.h"

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

			cubes[index]->SetDebugName("GridedTerrain" + to_string(index));
		}
	}

	tileDataBuffer = new TileDataBuffer;
	tileDataBuffer->SetDebugName("GridedTerrain");
	onCharacterBuffer = new IntValueBuffer;
	onCharacterBuffer->SetDebugName("GridedTerrain");
	tileColorBuffer = new ColorBuffer;
	tileColorBuffer->SetDebugName("GridedTerrain");

	Observer::Get()->AddEvent("BattleEnd", bind(&GridedTerrain::Reselect, this));

	Observer::Get()->AddEvent("CheckMovable", bind(&GridedTerrain::CheckMovableArea, this));

	Observer::Get()->AddEvent("SetStandingAttack", bind(&GridedTerrain::StandingAttack, this, true));
	Observer::Get()->AddEvent("UnsetStandingAttack", bind(&GridedTerrain::StandingAttack, this, false));

	Observer::Get()->AddParamEvent("CallPosToCoord", bind(&GridedTerrain::CalledPosToCoord, this, placeholders::_1));
}

GridedTerrain::~GridedTerrain()
{
	for (auto cube : cubes)
		delete cube;
		
	delete widthHeightBuffer;
	delete tileDataBuffer;
	delete onCharacterBuffer;
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
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		standingAttack = false;

	UpdateWorld();
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();

	auto holded = CharacterManager::Get()->HoldedCharacter();
	pair<int, int> holdedPos = {};
	pair<int, int> range = {};

	if (holded) {
		holdedPos = PosToCoord(holded->Pos());
		range = holded->GetAttackRange();
	}

	bool isEnemy = false;
	if (CharacterManager::Get()->HoldedCharacter())
		isEnemy = CharacterManager::Get()->HoldedCharacter()->GetStatus().teamNum == Character::Team::ENEMY;
	else {
		auto selectedCharacter = dynamic_cast<Character*>(ObjectOnIndex(selected));
		if (selectedCharacter)
			isEnemy = selectedCharacter->GetStatus().teamNum == Character::Team::ENEMY;
	}

	int index = 0;
	for (int i = 0; i < cubes.size(); i++) {
		if (!cubes[i]->Active())
			continue;

		auto characterOnIndex = dynamic_cast<Character*>(ObjectOnIndex(i));
		tileDataBuffer->SetOn(characterOnIndex != nullptr);
		if (characterOnIndex)
			tileDataBuffer->SetColor(characterOnIndex->GetStatus().teamNum == SRPGObject::Team::PLAYER ? Float4(0, 0, 1, 1) : Float4(1, 0, 0, 1));		

		if (holded && standingAttack) {
			auto coord = IndexToCoord(i);
			//ť�� ��ġ�� ��Ÿ� ������ Ȯ��
			int dist = abs(holdedPos.first - coord.first) + abs(holdedPos.second - coord.second);
			if (dist == 0)
				tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
			else if (dist >= range.first && dist <= range.second)
				tileColorBuffer->Get() = Float4(1.0f, 0.0f, 0.0f, 0.7f);
			else
				tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		}
		else {
			if (movables.find(i) != movables.end())
				tileColorBuffer->Get() = isEnemy ? Float4(1.0f, 0.0f, 0.0f, 0.7f) : Float4(0.0f, 0.0f, 1.0f, 0.7f);
			else if (attackables.find(i) != attackables.end())
				tileColorBuffer->Get() = Float4(1.0f, 0.0f, 0.0f, 0.7f);
			else
				tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		}

		tileDataBuffer->SetPS(8);
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

void GridedTerrain::AddObject(SRPGObject* object)
{
	for (auto o : objects) {
		if (o == object)
			return;
	}

	objects.push_back(object);
}

void GridedTerrain::SetSelected(int w, int h, bool forced)
{
	int s = CoordToIndex(w, h);
	if (s != selected || forced) {
		selected = s;

		if(CharacterManager::Get()->HoldedCharacter() == nullptr || forced)
			CheckMovableArea();

		Observer::Get()->ExcuteParamEvent("SetInfo", ObjectOnIndex(selected));
	}
}

void GridedTerrain::InputAction(int w, int h, SelectAction selectAction)
{
	//���� ���õ� ĳ���Ͱ� ����
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		selectAction = SELECT;
	
	SetSelected(w, h);

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

void GridedTerrain::InputAction(SelectAction selectAction)
{
	auto coord = IndexToCoord(selected);
	InputAction(coord.first, coord.second, selectAction);
}

void GridedTerrain::CheckMovableArea()
{
	//������Ʈ���� ��ǥ Ž��
	map<int, Transform*> objectsOnIndex;
	for (auto object : objects) {
		if (!object->Active())
			continue;

		auto coord = PosToCoord(object->GlobalPos());
		int index = CoordToIndex(coord.first, coord.second);
		objectsOnIndex[index] = object;
	}

	//���� ��ġ�� ������Ʈ�� �ִٸ� �ִ� �̵� �Ÿ� ���ϱ�
	int moveRange = 0;
	int team = -1;
	pair<int, int> attackRange = { 0, 0 };

	if (objectsOnIndex.find(selected) != objectsOnIndex.end()) {
		Character* character = dynamic_cast<Character*>(objectsOnIndex[selected]);
		if (character) {
			moveRange = character->GetMaxMove();
			team = character->GetStatus().teamNum;
			attackRange = character->GetAttackRange();
		}
	}

	movables.clear();

	const int dx[] = { 0,0,1,-1 };
	const int dy[] = { 1,-1,0,0 };

	pair<int, int> coord = IndexToCoord(selected);
	priority_queue<pair<int, int>> pq;
	pq.push({ 0, selected });
	movables[selected] = { 0, -1, true };

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

			bool standable = true;
			//�ش��ϴ� ��ġ�� �ٸ� ������Ʈ�� �ִ�
			if (objectsOnIndex.find(nIndex) != objectsOnIndex.end()) {
				//���� �� �����̸� ������ ���� �ִ�.(���������δ� ��� �Ұ�)
				auto c = (Character*)objectsOnIndex[nIndex];
				//ĳ���Ͱ� �ƴϰų� �ٸ� �� ĳ���͸� ����
				if (c == nullptr || c->GetStatus().teamNum != team) {
					continue;
				}
				
				//���������� �����ϰ�
				if (nIndex != selected) {
					//���� �� ĳ���Ͷ�� �� �� ���� ��ġ�� ����
					standable = false;
				}
				
			}
			//�̹� Ž���� Ÿ���̴�
			if (movables.find(nIndex) != movables.end() && movables[nIndex].dist <= nDist)
				continue;

			movables[nIndex] = { nDist, cur, standable };
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
		movables.clear();
		checkIndices.push_back(selected);
	}
	else {
		for (auto& movable : movables) {
			if(movable.second.standable)
				checkIndices.push_back(movable.first);
		}
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

				if (attackables.find(nIndex) != attackables.end()) {
					if (attackables[nIndex].first < movables[index].dist)
						continue;
				}
				attackables[nIndex] = make_pair(movables[index].dist, index);
			}
		}
	}
}

SRPGObject* GridedTerrain::ObjectOnIndex(int index)
{
	for (auto object : objects) {
		if (!object->Active())
			continue;

		int oi = CoordToIndex(PosToCoord(object->Pos()));
		if (oi == index)
			return object;
	}

	return nullptr;
}

void GridedTerrain::SelectCharacter(int w, int h)
{
	//�ش� ��ġ�� ������Ʈ �� ��ȿ�� ĳ���Ͱ� �ִ��� Ȯ��
	for (auto object : objects) {
		Character* character = (Character*)object;
		//������Ʈ�� ��ȿ�� ĳ�����ΰ�?
		if (character == nullptr || !character->Active())
			continue;

		//�� �÷��̾��� ĳ�����ΰ�?
		if (character->GetStatus().teamNum != TurnManager::Get()->GetCurPlayer())
			continue;

		//Ŀ�� ��ġ�� �ִ°�?
		pair<int, int> coord = PosToCoord(character->GlobalPos());
		if (coord.first != w || coord.second != h)
			continue;
				
		//���߿� �̱������� �� ���� ������ ��
		if (character->IsActed())
			continue;
		
		CharacterManager::Get()->CharacterHold(character, w, h);
		break;
	}

	//��ġ�� ���õ� ���� ����
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

	if (!movables[index].standable)
		return;

	if (movables[index].prev == -1) {
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
		index = movables[index].prev;
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
	if (w < 0 || w >= (int)col || h < 0 || h >= (int)row)
		return false;

	return cubes[CoordToIndex(w, h)]->Active();
}

vector<pair<SRPGObject*, pair<int, int>>> GridedTerrain::AttackableCharacters(int targetTeam)
{
	//attackables �������� ���ݹ��� ���� targetTeam ĳ���͸� �޴´�

	vector<pair<SRPGObject*, pair<int, int>>> result;
	for (auto object : objects) {
		auto character = (SRPGObject*)object;
		if (!character->Active())
			continue;

		if (character->GetStatus().teamNum != targetTeam)
			continue;

		auto coord = PosToCoord(character->GlobalPos());
		auto index = CoordToIndex(coord);
		if (attackables.find(index) == attackables.end())
			continue;

		result.push_back(make_pair(character, attackables[index]));
	}
	//�̵��Ÿ� ������ ����
	sort(result.begin(), result.end(), [](const pair<SRPGObject*, pair<int, int>>& l, const pair<SRPGObject*, pair<int, int>>& r) -> bool {
		return l.second.first < r.second.first;
	});

	return result;
}

void GridedTerrain::CalledPosToCoord(void* pack)
{
	pair<Vector3, pair<int, int>>* unpacked = (pair<Vector3, pair<int, int>>*)pack;

	unpacked->second = PosToCoord(unpacked->first);
}

void GridedTerrain::Reselect()
{
	auto coord = IndexToCoord(selected);

	SetSelected(coord.first, coord.second, true);
}
