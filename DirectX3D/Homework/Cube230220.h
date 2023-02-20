#pragma once
class Cube230220
{
public:
	Cube230220();
	~Cube230220();

	void Update();
	void Render();
	void RenderUI();
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;
	MatrixBuffer* worldBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;

	Float3 localPosition;
	Float3 localRotation;
	Float3 localScale = { 1.0f, 1.0f, 1.0f };
	Matrix S,R,T;
	Matrix world;
};

