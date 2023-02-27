#include "framework.h"
#include "Cube230223.h"

Cube230223::Cube230223(Vector3 size)
	: GameObject(L"Light/SpecularLight230223.hlsl")
{
	tag = "Cube";
	mesh = new Mesh<VertexType>;
	MakeMesh(size);
	mesh->CreateMesh();

	intensityBuffer = new FloatValueBuffer;
	intensityBuffer->Get()[0] = 1.0f;
}

Cube230223::~Cube230223()
{
	delete mesh;

	delete intensityBuffer;
}

void Cube230223::Update()
{
	if (!isActive)
		return;

	UpdateWorld();
}

void Cube230223::Render()
{
	if (!isActive)
		return;

	if (ambientMap)
		ambientMap->PSSet(2);
	intensityBuffer->SetPS(1);
	SetRender();
	mesh->Draw();
}

void Cube230223::RenderUI()
{
	if (ImGui::TreeNode(tag.c_str())) {
		Transform::RenderUI();
		material->RenderUI();

		string temp = tag + "_Shininess";
		ImGui::DragFloat(temp.c_str(), (float*)&intensityBuffer->Get()[0], 0.1f, 0.0f);

		temp = tag + "_Ambient";
		ImTextureID textureID = ambientMap ? ambientMap->GetSRV() : nullptr;
		if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
			DIALOG->OpenDialog(tag + "AmbientMap", "AmbientMap", ".png,.jpg", ".");
		}

		ImGui::TreePop();
	}

	if (DIALOG->Display(tag+"AmbientMap")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);

			file = file.substr(strlen(path) + 1, file.length());

			wstring textureFile = ToWString(file);
			ambientMap = textureFile.length() > 0 ? Texture::Add(textureFile) : nullptr;
		}

		DIALOG->Close();
	}
}

void Cube230223::MakeMesh(Vector3 size)
{
	vector<Vector3> points = {
		{-1, -1, -1},	//0
		{-1, +1, -1},	//1
		{+1, -1, -1},	//2
		{+1, +1, -1},	//3
		{-1, -1, +1},	//4
		{-1, +1, +1},	//5
		{+1, -1, +1},	//6
		{+1, +1, +1}	//7
	};

	vector<Vector3> normals = {
		{-1, -1, -1},	//0
		{-1, +1, -1},	//1
		{+1, -1, -1},	//2
		{+1, +1, -1},	//3
		{-1, -1, +1},	//4
		{-1, +1, +1},	//5
		{+1, -1, +1},	//6
		{+1, +1, +1}	//7
	};
	vector<vector<int>> squares = {
		{0,1,2,3},	//Front
		{1,5,3,7},	//Up
		{2,3,6,7},	//Right
		{6,7,4,5},	//Back
		{4,0,6,2},	//Bottom
		{4,5,0,1}	//Left
	};

	vector<Float2> uvs = {
		{0.0f, 1.0f},
		{0.0f, 0.0f},
		{1.0f, 1.0f},
		{1.0f, 0.0f}
	};

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve(4 * 6);

	for (auto& square : squares) {
		for (int i = 0; i < 4; i++) {
			VertexUVNormal vertex;
			vertex.pos = { size.x * points[square[i]].x, size.y * points[square[i]].y, size.z * points[square[i]].z };
			vertex.uv = uvs[i];
			vertex.normal = points[square[i]];

			vertices.push_back(vertex);
		}
	}

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve(6 * 6);
	for (int i = 0; i < 6; i++) {
		indices.push_back(i * 4);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 3);
	}
}
