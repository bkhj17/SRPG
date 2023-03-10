#include "framework.h"
#include "Model.h"

Model::Model(string name)
	: name(name)
{
	ReadMaterial();
	ReadMesh();

	worldBuffer = new MatrixBuffer();
}

Model::~Model()
{
	for (Material* material : materials)
		delete material;
	for (ModelMesh* mesh : meshes)
		delete mesh;

	delete worldBuffer;
}

void Model::Render()
{
	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	for (auto mesh : meshes)
		mesh->Render();
}

void Model::GUIRender()
{
	__super::GUIRender();

	for (auto material : materials)
		material->GUIRender();
}


void Model::SetShader(wstring file)
{
	for (auto material : materials)
		material->SetShader(file);
}

void Model::ReadMaterial()
{
	string file = "Models/Materials/" + name + "/" + name + ".mats";
	BinaryReader* reader = new BinaryReader(file);

	if (reader->IsFailed())
		assert(false);

	UINT size = reader->UInt();
	materials.reserve(size);

	for (int i = 0; i < size; i++) {
		Material* material = new Material;
		material->Load(reader->String());
		materials.push_back(material);
	}

	delete reader;
}

void Model::ReadMesh()
{
	string file = "Models/Meshes/" + name + ".mesh";
	BinaryReader* reader = new BinaryReader(file);
	if (reader->IsFailed())
		assert(false);

	UINT size = reader->UInt();
	meshes.reserve(size);

	for (int i = 0; i < size; i++) {
		auto mesh = new ModelMesh(reader->String());
		mesh->SetMaterial(materials[reader->UInt()]);

		UINT vertexCount = reader->UInt();
		ModelVertex* vertices = new ModelVertex[vertexCount];
		reader->Byte((void**)&vertices, sizeof(ModelVertex) * vertexCount);

		UINT indexCount = reader->UInt();
		UINT* indices = new UINT[indexCount];
		reader->Byte((void**)&indices, sizeof(UINT) * indexCount);

		mesh->CreateMesh(vertices, vertexCount, indices, indexCount);
		meshes.push_back(mesh);
	}

	size = reader->UInt();
	nodes.resize(size);
	for (auto& node : nodes) {
		node.index = reader->Int();
		node.name = reader->String();
		node.parent = reader->Int();
		node.transform = reader->Matrix();
	}

	size = reader->UInt();
	bones.resize(size);
	for (auto& bone : bones) {
		bone.index = reader->Int();
		bone.name = reader->String();
		bone.offset = reader->Matrix();
		boneMap[bone.name] = bone.index;
	}
	delete reader;
}
