#include "framework.h"
#include "GridedTerrain.h"

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

	Observer::Get()->AddEvent("BattleEnd", bind(&GridedTerrain::Reselect, this));

	Observer::Get()->AddEvent("CheckMovable", bind(&GridedTerrain::CheckMovableArea, this));

	Observer::Get()->AddEvent("SetStandingAttack", bind(&GridedTerrain::StandingAttack, this, true));
	Observer::Get()->AddEvent("UnsetStandingAttack", bind(&GridedTerrain::StandingAttack, this, false));

	Observer::Get()->AddParamEvent("CallPosToCoord", bind(&GridedTerrain::CalledPosToCoord, this, placeholders::_1));
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

	if (holded && standingAttack) {
		holdedPos = PosToCoord(holded->Pos());
		range = holded->GetAttackRange();
	}

	int index = 0;
	for (int i = 0; i < cubes.size(); i++) {
		if(!cubes[i]->Active())
			continue;

		if (holded && standingAttack) {
			auto coord = IndexToCoord(i);

			//큐브 위치가 사거리 안인지 확인
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
				tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
			else if (attackables.find(i) != attackables.end())
				tileColorBuffer->Get() = Float4(1.0f, 0.0f, 0.0f, 0.7f);
			else
				tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		}
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
	//아직 선택된 캐릭터가 없음
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
	//오브젝트들의 좌표 탐색
	map<int, Transform*> objectsOnIndex;
	for (auto object : objects) {
		if (!object->Active())
			continue;

		auto coord = PosToCoord(object->GlobalPos());
		int index = CoordToIndex(coord.first, coord.second);
		objectsOnIndex[index] = object;
	}

	//선택 위치에 오브젝트가 있다면 최대 이동 거리 구하기
	int moveRange = 0;
	int team = -1;
	pair<int, int> attackRange = { 0, 0 };

	if (objectsOnIndex.find(selected) != objectsOnIndex.end()) {
		Character* character = (Character*)objectsOnIndex[selected];
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

			//유효하지 않은 타일
			if (!cubes[nIndex]->Active())
				continue;

			bool standable = true;
			//해당하는 위치에 다른 오브젝트가 있다
			if (objectsOnIndex.find(nIndex) != objectsOnIndex.end()) {
				//같은 편 유닛이면 지나갈 수는 있다.(도착점으로는 사용 불가)
				auto c = (Character*)objectsOnIndex[nIndex];
				//캐릭터가 아니거나 다른 편 캐릭터면 막힘
				if (c == nullptr || c->GetStatus().teamNum != team) {
					continue;
				}
				
				//시작지점은 제외하고
				if (nIndex != selected) {
					//같은 편 캐릭터라면 설 수 없는 위치로 설정
					standable = false;
				}
				
			}
			//이미 탐색한 타일이다
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
	//공격범위 설정
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

Transform* GridedTerrain::ObjectOnIndex(int index)
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
	//해당 위치의 오브젝트 중 유효한 캐릭터가 있는지 확인
	for (auto object : objects) {
		Character* character = (Character*)object;
		//오브젝트가 유효한 캐릭터인가?
		if (character == nullptr || !character->Active())
			continue;

		//턴 플레이어의 캐릭터인가?
		if (character->GetStatus().teamNum != TurnManager::Get()->GetCurPlayer())
			continue;

		//커서 위치에 있는가?
		pair<int, int> coord = PosToCoord(character->GlobalPos());
		if (coord.first != w || coord.second != h)
			continue;
				
		//나중에 싱글톤으로 턴 정보 만들어야 함
		if (character->IsActed())
			continue;
		
		CharacterManager::Get()->CharacterHold(character, w, h);
		break;
	}

	//거치고도 선택된 것이 없음
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		CheckMovableArea();
}

void GridedTerrain::SelectMove(int w, int h)
{
	//이동 선택
	
	//이미 검사하고 들어오기는 하지만 그래도 일단 예외처리
	if (CharacterManager::Get()->HoldedCharacter() == nullptr)
		return;

	//커서 위치
	int index = CoordToIndex(w, h);
	//갈 수 있는 위치가 아닌데 그냥 커서만 올라가 있는 거
	if (movables.find(index) == movables.end())
		return;

	if (!movables[index].standable)
		return;

	if (movables[index].prev == -1) {
		//캐릭터 위치 그 자체 == 선택해제
		CharacterManager::Get()->CharacterUnhold();
		return;
	}

	//현재 위치(w, h)가 이동 가능한 위치
	vector<Vector3> path;
	while (index != -1) {
		pair<int, int> pathCoord = IndexToCoord(index);
		path.push_back(CoordToPos(pathCoord.first, pathCoord.second));
		//경로 역추적. 이걸 위해 movable.value.second를 이전 노드로 설정했다
		index = movables[index].prev;
	}
	//맨 뒤 == 현 위치
	path.pop_back();

	//character에 경로 째로 넘겨줘야 한다
	CharacterManager::Get()->HoldedCharacter()->SetMovePath(path);
}

void GridedTerrain::SelectAttack(int w, int h)
{
	//공격 선택

	//이미 검사하고 들어오기는 하지만 그래도 일단 예외처리
	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded == nullptr)
		return;

	auto holdedPos = PosToCoord(holded->Pos());
	auto range = holded->GetAttackRange();

	//커서 위치가 사거리 밖이면 패스
	int dist = abs(holdedPos.first - w) + abs(holdedPos.second - h);
	if (range.first > dist || range.second < dist)
		return;
	
	//커서 위치에 캐릭터가 있기는 한지 확인
	Character* attacked = nullptr;
	for (auto object : objects) {
		Character* character = (Character*)object;
		//캐릭터가 아니거나 비활성 상태면 패스
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

	//있으면 배틀 시작 - 진짜 시작할지는 매니저에서 판단하도록 하자
	CharacterManager::Get()->BattleStart(holded, attacked);
}

bool GridedTerrain::IsActiveCoord(int w, int h)
{
	if (w < 0 || w >= col || h < 0 || h >= row)
		return false;

	return cubes[CoordToIndex(w, h)]->Active();
}

vector<pair<Character*, pair<int, int>>> GridedTerrain::AttackableCharacters(int targetTeam)
{
	//attackables 기준으로 공격범위 내의 targetTeam 캐릭터를 받는다

	vector<pair<Character*, pair<int, int>>> result;
	for (auto object : objects) {
		auto character = (Character*)object;
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
	//이동거리 순으로 정렬
	sort(result.begin(), result.end(), [](const pair<Character*, pair<int, int>>& l, const pair<Character*, pair<int, int>>& r) -> bool {
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
