#include "framework.h"
#include "Terrain230222.h"

Terrain230222::Terrain230222()
	: width(MAX_VALUE), height(MAX_VALUE)
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
		int postWidth = width;
		int postHeight = height;
		ImGui::DragInt("Width", (int*)&width, 1.0f, 1, MAX_VALUE);
		ImGui::DragInt("Height", (int*)&height, 1.0f, 1, MAX_VALUE);

		if (width != postWidth || height != postHeight) {
			//메시 업데이트
			UpdateMesh();
		}

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
	SetupVertices(mesh->GetVertices(), mesh->GetIndices());

	mesh->UpdateVertex(mesh->GetVertices().data(), (UINT)mesh->GetVertices().size());
	mesh->UpdateIndices(mesh->GetIndices().data(), (UINT)mesh->GetIndices().size());
}

void Terrain230222::SetupVertices(vector<VertexType>& vertices, vector<UINT>& indices)
{

	vertices.resize((size_t)width * height);

	size_t index = 0;
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
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
	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			for (int i = 0; i < 6; i++) {
				indices[index++] = (width * (z + dxz[i].second) + x + dxz[i].first);
			}
		}
	}
}
