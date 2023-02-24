#pragma once
class Terrain230224 : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;
	const float MAX_HEIGHT = 20.0f;
public:
	Terrain230224();
	~Terrain230224();

	void Render();

	void GetHeight(Vector3& pos, Vector3& rot);
private:
	float interpolateHeight(Vector3& pos, Vector3& normal, VertexType& v0, VertexType& v1, VertexType& v2);

	void MakeNormal();
	void MakeMesh();


private:
	UINT width, height;
	Mesh<VertexType>* mesh;

	Texture* heightMap;
};

