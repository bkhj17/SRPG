#pragma once
class Cylinder230224 : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;
public:
	Cylinder230224(float radius = 1.0f, float height = 1.0f, int sliceCount = 30);
	~Cylinder230224();

	void Render();
private:
	void MakeMesh();

private:
	Mesh<VertexType>* mesh;
	float radius;
	float height;
	int sliceCount;
};

