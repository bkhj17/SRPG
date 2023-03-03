#include "framework.h"
#include "Terrain230224.h"

Terrain230224::Terrain230224()
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
	mesh->CreateMesh();
}

Terrain230224::~Terrain230224()
{
	delete mesh;
}

void Terrain230224::Render()
{
	SetRender();
	mesh->Draw();
}

void Terrain230224::GetHeight(Vector3& pos, Vector3& rot)
{
	int x = (int)pos.x;
	int z = (int)pos.z;
	if (x < 0 || x > (int)width || z < 0 || z > (int)height)
		return;
	
	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();

	VertexType& v0 = vertices[(size_t)z * width + x];
	VertexType& v1 = vertices[((size_t)z + 1) * width + x];
	VertexType& v2 = vertices[(size_t)z * width + x+1];
	VertexType& v3 = vertices[((size_t)z + 1) * width + x + 1];

	Vector3 normal;
	pos.y = interpolateHeight(pos, normal, v0, v1, v2, v3);

	Vector3 yAxis(0,1,0);

	float angle = Dot((Vector3&)v0.normal, yAxis);
	Vector3 axis = Cross((Vector3&)v0.normal, yAxis);
	if (axis.x == 0 && axis.y == 0 && axis.z == 0) {
		rot = axis;
		return;
	}

	rot = XMQuaternionRotationAxis(axis.GetNormalized(), angle);
	rot = XMQuaternionInverse(rot);
}

float Terrain230224::interpolateHeight(Vector3& pos, Vector3& normal, VertexType& v0, VertexType& v1, VertexType& v2, VertexType& v3)
{
	float dx = pos.x - v0.pos.x;
	float dz = pos.z - v0.pos.z;

	float D = 0.0f;

	VertexType vBase;
	if (dx + dz > 1.0f) {
		vBase = v3;
		dx = 1.0f - dx;
		dz = 1.0f - dz;
	}
	else
		vBase = v0;

	dx = 1.0f - dx;
	dz = 1.0f - dz;

	float sum = dx + dz;
	float rx = dx / sum;
	float rz = dz / sum;

	float f1 = abs(v1.pos.x - vBase.pos.x < FLT_EPSILON) ? 0.0f : dx / (v1.pos.x - vBase.pos.x);
	float f2 = abs(v2.pos.z - vBase.pos.z < FLT_EPSILON) ? 0.0f : dz / (v2.pos.z - vBase.pos.z);

	Vector3 n1 = Lerp((Vector3&)vBase.normal, (Vector3&)v1.normal, f1);
	Vector3 n2 = Lerp((Vector3&)vBase.normal, (Vector3&)v2.normal, f2);

	normal = (n1 + n2).GetNormalized();

	D = -Dot(normal, (Vector3&)vBase.pos);
	return (-normal.x * pos.x - normal.z * pos.z - D) / normal.y;
}

void Terrain230224::MakeNormal()
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

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
		//정규화는 셰이더에서 하니 생략

		//탄젠트
		Float2 uv0 = vertices[index0].uv;
		Float2 uv1 = vertices[index1].uv;
		Float2 uv2 = vertices[index2].uv;

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

void Terrain230224::MakeMesh()
{
	width = (UINT)heightMap->GetSize().x;
	height = (UINT)heightMap->GetSize().y;

	vector<Float4> pixels;
	heightMap->ReadPixels(pixels);

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((size_t)width * height);
	for (int z = 0; z < (int)height; z++) {
		for (int x = 0; x < (int)width; x++) {
			VertexType vertex;
			UINT index = width * z + x;
			vertex.pos = { (float)x,  pixels[index].x * MAX_HEIGHT, (float)z };
			vertex.uv = { x / (float)width - 1, z / (float)height - 1 };

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
	indices.reserve((size_t)(width - 1) * (height - 1) * 6);
	for (int z = 0; z < (int)height - 1; z++) {
		for (int x = 0; x < (int)width - 1; x++) {
			for (int i = 0; i < 6; i++)
				indices.push_back(width * (z + dxz[i].second) + x + dxz[i].first);
		}
	}
}
