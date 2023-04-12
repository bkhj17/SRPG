#pragma once
class ModelMesh
{
public:
	ModelMesh(string name);
	virtual ~ModelMesh();

	void Render();
	void RenderInstanced(UINT instanceCount);
	
	void CreateMesh(void* vertexData, UINT vertexCount, void* indexData, UINT indexCount);

	void SetMaterial(Material* material) { this->material = material; }
private:
	string name;

	Material* material = nullptr;
	Mesh<ModelVertex>* mesh = nullptr;
};

