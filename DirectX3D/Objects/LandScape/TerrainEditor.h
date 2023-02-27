#pragma once
class TerrainEditor : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;
	const float MAX_HEIGHT = 20.0f;
public:
	TerrainEditor();
	virtual ~TerrainEditor();

	virtual void Update();
	void Render() override;
	void RenderUI() override;

	Vector3 Picking();
protected:
	void MakeMesh();
	void MakeNormal();
	void MakeTangent();


protected:
	UINT width, height;
	Mesh<VertexType>* mesh;

	Texture* heightMap;

	Vector3 pickingPos;
};

