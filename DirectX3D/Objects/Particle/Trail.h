#pragma once
class Trail : public GameObject
{
public:
	Trail(wstring imageFile, Transform* start, Transform* end, UINT width, float speed = 100.0f);
	~Trail();

	void Update();
	void Render();

private:
	void CreateMesh();

private:
	Mesh<VertexUV>* mesh = nullptr;

	Transform *start, *end;

	UINT width;
	float speed = 100.0f;

	RasterizerState* rasterizerState[2] = {};
	BlendState* blendState[2] = {};
};

