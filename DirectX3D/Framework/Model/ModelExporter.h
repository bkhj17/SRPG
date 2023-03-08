#pragma once
class ModelExporter {
public:
	ModelExporter(string name, string file);
	~ModelExporter();

	void ExportMaterial();
	void ExportMesh();

private:
	void ReadMaterial();
	void WriteMaterial();
	string CreateTexture(string file);

	void ReadMesh(aiNode* node);
	void WriteMesh();

private:
	Assimp::Importer* importer;
	const aiScene* scene;

	string name;

	vector<Material*> materials;
	vector<MeshData*> meshes;
};