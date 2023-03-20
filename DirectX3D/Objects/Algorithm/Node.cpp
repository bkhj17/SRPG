#include "Framework.h"

Node::Node(Vector3 pos, int index)
	: index(index)
{
	Pos() = pos;
	UpdateWorld();
}

Node::~Node()
{
	for (auto edge : edges)
		delete edge;
}

void Node::Render()
{
	switch (state)
	{
	case Node::NONE:
		SetColor({ 0, 1, 1, 1 });
		break;
	case OPEN:
		SetColor({ 1, 0, 1, 1 });
		break;
	case CLOSED:
		SetColor({ 1, 1, 0, 1 });
		break;
	case USING:
		SetColor({ 0, 1, 0, 1 });
		break;
	case OBSTACLE:
		SetColor({ 1, 0, 0, 1 });
		break;
	default:
		break;
	}

	__super::Render();
}

void Node::AddEdge(Node* node)
{
	Edge* edge = new Edge;
	edge->index = node->index;
	edge->cost = Distance(node->GlobalPos(), GlobalPos());
	edges.push_back(edge);
}
