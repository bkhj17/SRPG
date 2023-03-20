#pragma once
class Node : public BoxCollider
{
public:
	friend class AStar;

	enum State {
		NONE, OPEN, CLOSED, USING, OBSTACLE 
	};

	struct Edge {
		int index = -1;
		float cost = FLT_MAX;
	};
public:
	Node(Vector3 pos, int index);
	~Node();

	void Render();
	void AddEdge(Node* node);

	void SetState(State state) { this->state = state; }
	float GetCost() { return f; }
private:

	int index = 0;
	int via = -1;

	float f = 0, g = 0, h = 0;

	State state = NONE;
	vector<Edge*> edges;
};

