#pragma once

class Obstacle : public SRPGObject
{
private:
	const float CURSOR_HEIGHT = 25.0f;
public:
	Obstacle();
	~Obstacle();

	void Update();
	void Render();

	void Damaged(int damage) override;

	void SetShader(wstring shader) { model->SetShader(shader); }

	virtual float GetCursorHeight() override { return CURSOR_HEIGHT; }
private:
	Model* model;
};

