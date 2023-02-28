#include "framework.h"
#include "Terrain.h"

Terrain::Terrain()
	: width(10), height(10)
{
	tag = "Terrain";

	material->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	material->SetSpecularMap(L"Textures/Landscape/fieldstone_SM.tga");
	material->SetNormalMap(L"Textures/Landscape/fieldstone_NM.tga");
	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	MakeTangent();
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

float Terrain::GetHeight(const Vector3& pos, Vector3* normal) const
{
	//global은 무시. Terrain이 원점임을 가정한다
	int x = (int)pos.x;
	int z = (int)(height - pos.z);

	if (x < 0 || x >= width - 1) return 0.0f;
	if (z < 0 || z >= width - 1) return 0.0f;

	UINT index[4];
	index[0] = width * z + x;
	index[1] = width * (z+1) + x;
	index[2] = width * z + (x+1);
	index[3] = width * (z+1) + (x+1);

	vector<VertexType>& vertices = mesh->GetVertices();

	Vector3 p[4];
	for (UINT i = 0; i < 4; i++)
		p[i] = vertices[index[i]].pos;
	
	float u = pos.x - p[0].x;
	float v = pos.z - p[0].z;

	Vector3 result;
	if (u + v <= 1.0f) {
		result = (p[2] - p[0]) * u + (p[1] - p[0]) * v + p[0];

		if(normal)
			*normal = GetNormalFromPolygon(p[0], p[1], p[2]);
		return result.y;
	}
	else {
		u = 1.0f - u;
		v = 1.0f - v;

		result = (p[2] - p[3]) * u + (p[1] - p[3]) * v + p[3];

		if (normal)
			*normal = GetNormalFromPolygon(p[2], p[1], p[3]);
		return result.y;
	}

	return 0.0f;
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
			//vertex.pos = { (float)x,  pixels[index].x * MAX_HEIGHT, (float)z };
			//vertex.uv = { x/(float)(width-1), 1.0f-z/(float)(height-1) };
			vertex.pos = { (float)x,  pixels[index].x * MAX_HEIGHT, height - (float)z };
			vertex.uv = { x / (float)(width - 1), z / (float)(height - 1) };
			vertices.push_back(vertex);
		}
	}
	/*
	vector<pair<int, int>> dxz = {
		{0,0},
		{0,1},
		{1,0},
		{1,0},
		{0,1},
		{1,1}
	};
	*/
	vector<pair<int, int>> dxz = {
		{0,0},
		{1,0},
		{0,1},
		{0,1},
		{1,0},
		{1,1}
	};

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve((size_t)(width - 1) * (height - 1) * 6);
	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			for (int i = 0; i < 6; i++)
				indices.push_back(width * (z + dxz[i].second) + x + dxz[i].first);
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

void Terrain::MakeNormal()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();
	for (UINT i = 0; i < indices.size() / 3; i++) {
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 pos0 = vertices[index0].pos;
		Vector3 pos1 = vertices[index1].pos;
		Vector3 pos2 = vertices[index2].pos;

		//순서 주의
		Vector3 e0 = pos1 - pos0;
		Vector3 e1 = pos2 - pos0;

		Vector3 normal = Cross(e0, e1).GetNormalized();

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
		//정규화는 셰이더에서 하니 생략
	}
}


void Terrain::MakeTangent()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (int i = 0; i * 3 < indices.size(); i++) {
		int index0 = indices[i * 3];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		Float2 uv0 = vertices[index0].uv;
		Float2 uv1 = vertices[index1].uv;
		Float2 uv2 = vertices[index2].uv;

		Vector3 e0 = (Vector3)vertices[index1].pos - vertices[index0].pos;
		Vector3 e1 = (Vector3)vertices[index2].pos - vertices[index0].pos;

		float u1 = uv1.x - uv0.x;
		float v1 = uv1.y - uv0.y;
		float u2 = uv2.x - uv0.x;
		float v2 = uv2.y - uv0.y;

		float d = 1.0f / (u1 * v2 - v1 * u2);
		Vector3 tangent = d * (e0 * v2 - e1 * v1);
		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}
}
