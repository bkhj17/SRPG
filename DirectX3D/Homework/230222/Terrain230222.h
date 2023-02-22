#pragma once
class Terrain230222 : public GameObject
{
private:
	typedef VertexUV VertexType;

	const float MAX_VALUE = 100;

public:
	Terrain230222();
	~Terrain230222();

	void Render();
	void RenderUI();
private:
	void CreateMesh();
	void UpdateMesh();

	void SetupVertices(vector<VertexType>& vertices, vector<UINT>& indices);
private:
	UINT width, height;
	Mesh<VertexType>* mesh;
};

