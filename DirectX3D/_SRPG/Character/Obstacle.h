#pragma once

class Obstacle : public SRPGObject
{
public:
	Obstacle();
	~Obstacle();

	void Update();
	void Render();

	void Damaged(int damage) override;
private:
	Model* model;
};

