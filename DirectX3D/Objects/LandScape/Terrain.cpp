#include "framework.h"
#include "Terrain.h"

Terrain::Terrain()
	: width(10), height(10)
{
	material->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	material->SetSpecularMap(L"Textures/Landscape/fieldstone_SM.tga");
	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	mesh->CreateMesh();
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

void Terrain::MakeNormal()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();
	for (UINT i = 0; i < indices.size() / 3; i++) {
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].pos;
		Vector3 v1 = vertices[index1].pos;
		Vector3 v2 = vertices[index2].pos;

		//순서 주의
		Vector3 e0 = v1 - v0;
		Vector3 e1 = v2 - v0;

		Vector3 normal = Cross(e0, e1).GetNormalized();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
		//정규화는 셰이더에서 하니 생략
	}
}

void Terrain::MakeMesh()
{
	width = (UINT)heightMap->GetSize().x;
	height = (UINT)heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);
	
	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((size_t)width * height);
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			VertexType vertex;
			UINT index = width * z + x;
			vertex.pos = { (float)x,  pixels[index].x * MAX_HEIGHT, (float)z };
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
}
