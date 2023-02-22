#pragma once
class Terrain : public GameObject
{
private:
	typedef VertexUV VertexType;

public:
	Terrain();
	~Terrain();

	void Render();
private:
	void CreateMesh();

private:
	UINT width, height;
	Mesh<VertexType>* mesh;
};

