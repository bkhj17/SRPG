#pragma once
class Model : public Transform
{
public:
	Model(string name);
	~Model();

	void Render();
	void GUIRender();

	void SetShader(wstring file);
	Material* AddMaterial(string name = "");
	int GetMeshNum() { return meshes.size(); }
	ModelMesh* GetMesh(UINT index) { return meshes[index]; }


	string GetName() { return name; }
	vector<Material*> GetMatetials() { return materials; }
private:
	void ReadMaterial();
	void ReadMesh();

protected:
	string name;
	vector<Material*> materials;
	vector<ModelMesh*> meshes;
	vector<NodeData> nodes;
	vector<BoneData> bones;

	map<string, UINT> boneMap;
	WorldBuffer* worldBuffer;
};

