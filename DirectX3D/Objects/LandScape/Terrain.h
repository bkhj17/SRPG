#pragma once
class Terrain : public GameObject
{
protected:
	typedef VertexUVNormalTangent VertexType;
	const float MAX_HEIGHT = 20.0f;
public:
	Terrain();
	Terrain(string mapFile);
	~Terrain();

	void Render();

	float GetHeight(const Vector3& pos, Vector3* normal = nullptr) const;
	Vector3 Picking();

	Vector2 GetSize() { return Vector2((float)width, (float)height); }
private:
	void MakeNormal();
	void MakeMesh();
	void MakeTangent();

protected:
	UINT width, height;
	Mesh<VertexType>* mesh;

	Texture* heightMap;
	Texture* alphaMap;
	Texture* secondMap;
	Texture* thirdMap;
};