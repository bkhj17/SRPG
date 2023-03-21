#include "Framework.h"

AStar::AStar(UINT width, UINT height)
	: width(width), height(height)
{
	heap = new Heap;
}

AStar::~AStar()
{
	for (auto node : nodes)
		delete node;

	delete heap;
}

void AStar::Update()
{
	if (KEY_DOWN(VK_RBUTTON)) {
		Ray ray = CAM->ScreenPointToRay(mousePos);
		for (Node* node : nodes) {
			if (node->IsRayCollision(ray)) {
				node->SetState(Node::OBSTACLE);
				break;
			}
		}
	}
}

void AStar::Render()
{
	for (auto node : nodes)
		node->Render();
}

void AStar::SetNode(Terrain* terrain)
{
	Vector2 size = terrain->GetSize();

	intervel = { size.x / width, size.y / height };
	for (UINT z = 0; z <= height; z++) {
		for (UINT x = 0; x <= width; x++) {
			Vector3 pos = Vector3(x * intervel.x, 0, z * intervel.y);
			pos.y = terrain->GetHeight(pos);

			nodes.push_back(new Node(pos, nodes.size()));
			nodes.back()->Scale() = {intervel.x, 50.0f, intervel.y};
			nodes.back()->UpdateWorld();

			if (pos.y > 0.0f) {
				nodes.back()->SetState(Node::OBSTACLE);
				AddObstacle(nodes.back());
			}
		}
	}

	SetEdge();
}

int AStar::FindCloseNode(Vector3 pos)
{
	float minCost = FLT_MAX;
	int index = -1;
	for (UINT i = 0; i < nodes.size(); i++) {
		if (nodes[i]->state == Node::OBSTACLE)
			continue;

		float cost = Distance(nodes[i]->GlobalPos(), pos);
		if (minCost > cost) {
			minCost = cost;
			index = i;
		}
	}

	return index;
}

int AStar::FindRandomPos(Vector3 pos, float range)
{
	vector<Node*> findNodes;

	for (Node* node : nodes) {
		float distance = Distance(pos, node->GlobalPos());
		if (distance < range && node->state != Node::OBSTACLE) {
			findNodes.push_back(node);
		}
	}

	Node* findNode = findNodes[Random(0, findNodes.size())];

	return findNode->index;
}


void AStar::GetPath(IN int start, IN int end, OUT vector<Vector3>& path)
{
	Reset();
	path.clear();

	if (start == end)
		return;

	float G = 0;
	float H = GetManhattanDistance(start, end);

	nodes[start]->f = G + H;
	nodes[start]->g = G;
	nodes[start]->h = H;
	nodes[start]->via = start;
	nodes[start]->state = Node::OPEN;

	heap->Insert(nodes[start]);
	while (nodes[end]->state != Node::CLOSED) {
		if (heap->Empty())
			return;

		int curIndex = GetMinNode();
		Extend(curIndex, end);
		nodes[curIndex]->state = Node::CLOSED;
	}

	int curIndex = end;
	while (curIndex >= 0 && curIndex != nodes[curIndex]->via) {
		nodes[curIndex]->state = Node::USING;
		path.push_back(nodes[curIndex]->GlobalPos());
		curIndex = nodes[curIndex]->via;
	}
}

void AStar::MakeDirectPath(IN Vector3 start, IN Vector3 end, OUT vector<Vector3>& path)
{
	int cutNodeNum = 0;
	Ray ray;
	ray.pos = start;

	//장애물을 만나지 않는 가장 먼 노드를 찾는다
	for (int i = 0; i < path.size(); i++) {
		if (!IsCollisionObstacle(start, path[i])) {
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for (int i = 0; i < cutNodeNum; i++)
		path.pop_back();

	cutNodeNum = 0;
	ray.pos = end;

	for (int i = 0; i < path.size(); i++) {
		if (!IsCollisionObstacle(end, path[path.size() - i - 1])) {
			cutNodeNum = path.size() - i - 1;
			break;
		}
	}

	for(int i = 0; i < cutNodeNum; i++)
		path.erase(path.begin());
	path.insert(path.begin(), end);

}

bool AStar::IsCollisionObstacle(Vector3 start, Vector3 end)
{
	Ray ray(start, (end - start).GetNormalized());
	float distance = (end - start).Length();

	Contact contact;
	for (auto obstacle : obstacles) {
		if (obstacle->IsRayCollision(ray, &contact)) {
			if (contact.distance < distance) {
				return true;
			}
		}
	}

	return false;
}

void AStar::AddObstacle(Collider* collider)
{
	if (!collider)
		return;
	obstacles.push_back(collider);
}

void AStar::Reset()
{
	for (Node* node : nodes) {
		if (node->state != Node::OBSTACLE)
			node->state = Node::NONE;
	}

	heap->Clear();
}

float AStar::GetManhattanDistance(int start, int end)
{
	//dx + dy
	Vector3 startPos = nodes[start]->GlobalPos();
	Vector3 endPos = nodes[end]->GlobalPos();

	Vector3 temp = endPos - startPos;

	float x = abs(temp.x);
	float z = abs(temp.z);

	if (isDiagonal) {
		float mx = max(x, z);
		float mn = min(x, z);
		return mx - mn + mn * sqrtf(2.0f);
	}
	else
		return abs(temp.x) + abs(temp.y);
}

void AStar::Extend(int center, int end)
{
	for (auto edge : nodes[center]->edges) {
		int index = edge->index;
		//지나온
		if (nodes[index]->state == Node::CLOSED)
			continue;
		//못 감
		if (nodes[index]->state == Node::OBSTACLE)
			continue;

		float G = nodes[center]->g + edge->cost;
		float H = GetManhattanDistance(index, end);
		float F = G + H;

		if (nodes[index]->state == Node::OPEN) {
			if (F < nodes[index]->f) {
				nodes[index]->g = G;
				nodes[index]->f = F;
				nodes[index]->via = center;
			}
		}
		else if (nodes[index]->state == Node::NONE) {
			nodes[index]->h = H;
			nodes[index]->g = G;
			nodes[index]->f = F;
			nodes[index]->via = center;
			nodes[index]->state = Node::OPEN;
			//openNodes.push_back(index);
			heap->Insert(nodes[index]);
		}
	}
}

int AStar::GetMinNode()
{
	/*
	int openIndex = 0;


	int nodeIndex = openNodes[openIndex];
	float minF = nodes[nodeIndex]->f;

	for (UINT i = 0; i < openNodes.size(); i++) {
		nodeIndex = openNodes[i];
		if (nodes[nodeIndex]->f < minF) {
			minF = nodes[nodeIndex]->f;
			openIndex = i;
		}
	}

	nodeIndex = openNodes[openIndex];
	openNodes.erase(openNodes.begin() + openIndex);
	return nodeIndex;
	*/
	return heap->DeleteRoot()->index;
}

void AStar::SetEdge()
{
	//y * width + x
	SetEdgeDiagonal();
}

void AStar::SetEdgeDiagonal()
{
	size_t width = (size_t)this->width + 1;
	for (size_t i = 0; i < nodes.size(); i++) {
		if (i % width != width - 1)
		{
			nodes[i]->AddEdge(nodes[i + 1]);
			nodes[i + 1]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width)
		{
			nodes[i]->AddEdge(nodes[i + width]);
			nodes[i + width]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width && i % width != width - 1)
		{
			nodes[i]->AddEdge(nodes[i + width + 1]);
			nodes[i + width + 1]->AddEdge(nodes[i]);
		}

		if (i < nodes.size() - width && i % width != 0)
		{
			nodes[i]->AddEdge(nodes[i + width - 1]);
			nodes[i + width - 1]->AddEdge(nodes[i]);
		}
	}
}
