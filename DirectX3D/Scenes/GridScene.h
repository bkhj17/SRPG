#pragma once

class GridScene : public Scene
{
private:
	const int MAX_VALUE = 100;
public:
	GridScene();
	~GridScene();
	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	void CreateMesh();

private:
	Material* material;
	Mesh<VertexColor>* mesh;

	UINT width, height;

	MatrixBuffer* worldBuffer;

};

