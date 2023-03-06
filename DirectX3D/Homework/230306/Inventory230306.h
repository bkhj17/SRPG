#pragma once
class Inventory230306 : public Singleton<Inventory230306>
{
private:
	friend class Singleton;
	Inventory230306();
	~Inventory230306();

public:
	void Input(Texture* texture);
	Texture* Select();
	void OutputSelect();

	void Update();
	void Render();

private:
	vector<pair<Quad*, int>> inventory;

	Vector2 textureSize = { 100.0f, 100.0f };
	Vector3 startPos = { 100.0f, 100.0f, 0.0f};
	Vector3 posStep = { 50.0f, 0.0f, 0.0f };

	int select = -1;
	IntValueBuffer* selectBuffer;
};

