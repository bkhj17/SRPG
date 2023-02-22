#pragma once
class Cube : public GameObject
{
private:
	typedef VertexUV VertexType;

public:
	Cube(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	virtual ~Cube();

	virtual void Update();
	virtual void Render();
private:
	Mesh<VertexUV>* mesh;
};

