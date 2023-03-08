#pragma once
class Model : public Transform
{
public:
	Model(string name);
	~Model();

	void Render();
	void GUIRender();

	void SetShader(wstring file);

private:
	void ReadMaterial();
	void ReadMesh();

private:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;

	MatrixBuffer* worldBuffer;
};

