#include "framework.h"
#include "ModelExporter.h"

ModelExporter::ModelExporter(string name, string file)
	: name(name)
{
	importer = new Assimp::Importer();
	
	//필요한 정보 빼고 제외 
	importer->SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

	scene = importer->ReadFile(file, 
		aiProcess_ConvertToLeftHanded 
		| aiProcessPreset_TargetRealtime_MaxQuality);
	assert(scene != nullptr);
}

ModelExporter::~ModelExporter()
{
	delete importer;
}

void ModelExporter::ExportMaterial()
{
	ReadMaterial();
	WriteMaterial();
}

void ModelExporter::ExportMesh()
{
	ReadMesh(scene->mRootNode);
	WriteMesh();
}

void ModelExporter::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++) {
		aiMaterial* srcMaterial = scene->mMaterials[i];
		Material* material = new Material();

		material->GetName() = srcMaterial->GetName().C_Str();

		aiColor3D color;
		Material::MaterialBuffer::Data& data = material->GetData();

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		data.diffuse = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		data.specular = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		data.ambient = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		data.emissive = Float4(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_SHININESS, data.shininess);

		aiString file;
		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		material->SetDiffuseMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		material->SetSpecularMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		material->SetNormalMap(ToWString(CreateTexture(file.C_Str())));
		file.Clear();

		materials.push_back(material);
	}
}

void ModelExporter::WriteMaterial()
{
	string file = name + ".mats";
	string savePath = "Models/Materials/" + name + "/" + file;

	CreateFolders(savePath);

	BinaryWriter* writer = new BinaryWriter(savePath);

	writer->UInt(materials.size());
	for (Material* material : materials) {
		string path = savePath + material->GetName() + ".mat";
		material->Save(path);

		writer->String(path);

		delete material;
	}

	materials.clear();

	delete writer;
}

string ModelExporter::CreateTexture(string file)
{
	if (file.length() == 0)
		return "";

	string fileName = GetFileNameWithoutExtension(file) + ".png";
	string path = "Textures/Model/" + name + "/" + fileName;

	CreateFolders(path);

	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());
	if (texture == nullptr)
		return "";

	if (texture->mHeight < 1) {
		BinaryWriter w(path);
		w.Byte(texture->pcData, texture->mWidth);
	}
	else {
		Image image;
		image.width = texture->mWidth;
		image.height = texture->mHeight;
		image.pixels = (uint8_t*)texture->pcData;
		image.rowPitch = image.width * 4;
		image.slicePitch = image.width * image.height * 4;
		SaveToWICFile(image, WIC_FLAGS_NONE, 
			GetWICCodec(WIC_CODEC_PNG), ToWString(path).c_str());
	}

	return path;
}

void ModelExporter::ReadMesh(aiNode* node)
{
	for (UINT i = 0; i < node->mNumMeshes; i++) {
		MeshData* mesh = new MeshData;
		mesh->name = node->mName.C_Str();

		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		mesh->materialIndex = srcMesh->mMaterialIndex;
		mesh->vertices.resize(srcMesh->mNumVertices);
		for (UINT v = 0; v < srcMesh->mNumVertices; v++) {
			ModelVertex vertex;
			memcpy(&vertex.pos, &srcMesh->mVertices[v], sizeof(Float3));

			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.uv, &srcMesh->mTextureCoords[0][v], sizeof(Float2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.normal, &srcMesh->mNormals[v], sizeof(Float3));

			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.tangent, &srcMesh->mTangents[v], sizeof(Float3));

			mesh->vertices[v] = vertex;
		}

		mesh->indices.resize(srcMesh->mNumFaces * 3);
		for (UINT f = 0; f < srcMesh->mNumFaces; f++) {
			aiFace& face = srcMesh->mFaces[f];
			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->indices[(size_t)f * 3 + k] = face.mIndices[k];
		}

		meshes.push_back(mesh);
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadMesh(node->mChildren[i]);
}

void ModelExporter::WriteMesh()
{
	string path = "Models/Meshes/" + name + ".mesh";
	CreateFolders(path);
	BinaryWriter* writer = new BinaryWriter(path);

	writer->UInt(meshes.size());
	for (auto mesh : meshes) {
		writer->String(mesh->name);
		writer->UInt(mesh->materialIndex);

		writer->UInt(mesh->vertices.size());
		writer->Byte(mesh->vertices.data(), sizeof(ModelVertex) * mesh->vertices.size());

		writer->UInt(mesh->indices.size());
		writer->Byte(mesh->indices.data(), sizeof(UINT) * mesh->indices.size());

		delete mesh;
	}
	meshes.clear();
	delete writer;
}
