#pragma once
class Terrain : public GameObject
{
private:
	typedef VertexUVNormal VertexType;
	const float MAX_HEIGHT = 20.0f;
public:
	Terrain();
	~Terrain();

	void Render();
private:
	void MakeNormal();
	void MakeMesh();


private:
	UINT width, height;
	Mesh<VertexType>* mesh;

	Texture* heightMap;
};