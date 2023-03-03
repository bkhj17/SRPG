#include "framework.h"
#include "Terrain230222.h"

Terrain230222::Terrain230222()
	: width(DEFAULT_VALUE), height(DEFAULT_VALUE)
{
	material->SetDiffuseMap(L"Textures/Landscape/Dirt.png");

	CreateMesh();
}

Terrain230222::~Terrain230222()
{
	delete mesh;
}

void Terrain230222::Render()
{
	SetRender();
	mesh->Draw();
}

void Terrain230222::RenderUI()
{
	if (ImGui::TreeNode("TerrainOption")) {
		int pastWidth = width;
		int pastHeight = height;
		ImGui::DragInt("Width", (int*)&width, 1.0f, 1);
		ImGui::DragInt("Height", (int*)&height, 1.0f, 1);

		material->RenderUI();

		if (width != pastWidth || height != pastHeight) {
			//메시 업데이트
			UpdateMesh();
		}

		if (ImGui::Button("Save"))
			Save();

		ImGui::SameLine();

		if (ImGui::Button("Load"))
			Load();

		ImGui::TreePop();
	}
}

void Terrain230222::CreateMesh()
{
	mesh = new Mesh<VertexType>;
	SetupVertices(mesh->GetVertices(), mesh->GetIndices());
	mesh->CreateMesh();
}

void Terrain230222::UpdateMesh()
{
	if (width * height > maxSize) {
		delete mesh;
		CreateMesh();
	}
	else {
		SetupVertices(mesh->GetVertices(), mesh->GetIndices());

		mesh->UpdateVertex();
		mesh->UpdateIndex();
	}
}

void Terrain230222::Save()
{
	BinaryWriter* writer = new BinaryWriter("TextData/Transforms/" + tag + ".srt");

	writer->UInt(width);
	writer->UInt(height);

	delete writer;
}

void Terrain230222::Load()
{

	BinaryReader* reader = new BinaryReader("TextData/Transforms/" + tag + ".srt");

	if (reader->IsFailed()) 
		return;

	width = reader->UInt();
	height = reader->UInt();

	UpdateMesh();

	delete reader;
}

void Terrain230222::SetupVertices(vector<VertexType>& vertices, vector<UINT>& indices)
{
	maxSize = max(maxSize, width * height);

	vertices.resize((size_t)width * height);

	size_t index = 0;
	for (int z = 0; z < (int)height; z++) {
		for (int x = 0; x < (int)width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv = { x / ((float)width - 1), z / ((float)height - 1) };
			vertices[index++] = vertex;
		}
	}

	vector<pair<int, int>> dxz = {
		{0,0},
		{0,1},
		{1,0},
		{1,0},
		{0,1},
		{1,1}
	};

	indices.resize((size_t)(width - 1) * (height - 1) * 6);
	index = 0;
	for (int z = 0; z < (int)height - 1; z++) {
		for (int x = 0; x < (int)width - 1; x++) {
			for (int i = 0; i < 6; i++) 
				indices[index++] = ((int)width * (z + dxz[i].second) + x + dxz[i].first);
		}
	}
}
