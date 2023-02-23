#pragma once
class GridScene230222 : public Scene
{
private:
	const int DEFAULT_VALUE = 200;
public:
	GridScene230222();
	~GridScene230222();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateMesh();
	void UpdateMesh();
	void SetUpVertices(vector<VertexColor>& vertices);
private:
	Material* material;
	Mesh<VertexColor>* mesh;

	UINT maxSize;
	UINT width, height;

	MatrixBuffer* worldBuffer;

	bool renderGrid = true;
};

