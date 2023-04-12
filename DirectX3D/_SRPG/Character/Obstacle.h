#pragma once

class Obstacle : public SRPGObject
{
private:
	friend class CharacterManager;

	const float CURSOR_HEIGHT = 25.0f;
	Obstacle();
	~Obstacle();
public:

	void Update();
	void Render();

	void Damaged(int damage) override;

	void SetShader(wstring shader) { model->SetShader(shader); }

	virtual float GetCursorHeight() override { return CURSOR_HEIGHT; }
private:
	Model* model;
};

