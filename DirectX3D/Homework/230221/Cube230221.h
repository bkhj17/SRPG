#pragma once
class Cube230221 : public GameObject
{
public:
	Cube230221(wstring fileName, Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~Cube230221();

	virtual void Update();
	virtual void Render();
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexUV> vertices;
	vector<UINT> indices;

	Texture* texture;
};

