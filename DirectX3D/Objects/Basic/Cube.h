#pragma once
class Cube : public GameObject
{
public:
	Cube(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	virtual ~Cube();

	virtual void Update();
	virtual void Render();

private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexColor> vertices;
	vector<UINT> indices;
};

