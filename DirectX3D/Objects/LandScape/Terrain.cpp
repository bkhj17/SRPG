#include "framework.h"
#include "Terrain.h"

Terrain::Terrain()
	: width(10), height(10)
{
	material->SetDiffuseMap(L"Textures/Landscape/Dirt.png");


	CreateMesh();
}

Terrain::~Terrain()
{
	delete mesh;
}

void Terrain::Render()
{
	SetRender();
	mesh->Draw();
}

void Terrain::CreateMesh()
{
	mesh = new Mesh<VertexType>;

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((size_t)width * height);
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)z };
			vertex.uv = { x/(float)width-1, z/(float)height-1 };
			vertices.push_back(vertex);
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
	
	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve((size_t)(width-1) * (height-1) * 6);
	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			for (int i = 0; i < 6; i++)
				indices.push_back(width * (z+dxz[i].second) + x+dxz[i].first);
			/*
			indices.push_back(width * z + x);
			indices.push_back(width * (z+1) + x);
			indices.push_back(width * z + (x+1));
			indices.push_back(width * z + (x+1));
			indices.push_back(width * (z+1) + x);
			indices.push_back(width * (z+1) + (x+1));
			*/
		}
	}
	mesh->CreateMesh();
}
