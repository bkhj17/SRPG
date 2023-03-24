#include "framework.h"
#include "Fox.h"

Fox::Fox() : ModelAnimator("Fox")
{
	ReadClip("Idle", 0);
	ReadClip("Run", 0, "FoxTransform");

	PlayClip(curState);
}

Fox::~Fox()
{
}

void Fox::Update()
{
	Control();
	Move();
	Rotate();

	ModelAnimator::Update();
}

void Fox::Render()
{
	__super::Render();
}

void Fox::Control()
{
	if (KEY_DOWN(VK_LBUTTON)) {
		destPos = terrain->Picking();

		if (astar->IsCollisionObstacle(GlobalPos(), destPos)) {
			SetPath();
		} else {
			path.clear();
			path.push_back(destPos);
		}
	}
}

void Fox::Move()
{
	if (path.empty()) {
		SetState(IDLE);
		return;
	}
	
	SetState(RUN);
	Vector3 dest = path.back();
	Vector3 direction = dest - GlobalPos();
	direction.y = 0.0f;

	if (direction.Length() < 1.0f) {
		path.pop_back();
	}

	velocity = direction.GetNormalized();
	Pos() += velocity * moveSpeed * DELTA;
}

void Fox::Rotate()
{
	if (curState == IDLE) return;


	Vector3 cross = Cross((Vector3&)Forward(), velocity);
	if (cross.y < 0)
		Rot().y += rotSpeed * DELTA;
	else if (cross.y > 0)
		Rot().y -= rotSpeed * DELTA;
}

void Fox::SetState(State state)
{
	if (curState == state)
		return;

	curState = state;
	PlayClip(curState);
}

void Fox::SetPath()
{
	int start = astar->FindCloseNode(GlobalPos());
	int end = astar->FindCloseNode(destPos);

	astar->GetPath(start, end, path);
	astar->MakeDirectPath(GlobalPos(), destPos, path);


	size_t pathSize = path.size();

	while (path.size() > 2) {
		vector<Vector3> tempPath = path;
		tempPath.erase(tempPath.begin());
		tempPath.pop_back();

		Vector3 start = path.back();
		Vector3 end = path.front();

		astar->MakeDirectPath(start, end, tempPath);
		path.clear();

		path = tempPath;
		path.push_back(start);

		if (pathSize == path.size())
			break;
		else
			pathSize = path.size();
	}
	path.insert(path.begin(), destPos);
}
