#pragma once

class Fox : public ModelAnimator
{
private:
	enum State {
		IDLE, RUN
	};

public:
	Fox();
	~Fox();

	void Update();
	void Render();

	void SetTerrain(Terrain* terrain) { this->terrain = terrain; }
	void SetAStar(AStar* astar) { this->astar = astar; }
private:
	void Control();
	void Move();
	void Rotate();

	void SetState(State state);
	void SetPath();

private:
	State curState = IDLE;

	float moveSpeed = 100.0f;
	float rotSpeed = 10.0f;

	Vector3 velocity;
	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain = nullptr;
	AStar* astar = nullptr;
};

