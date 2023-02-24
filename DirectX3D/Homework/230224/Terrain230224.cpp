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
	if (x < 0 || x > width || z < 0 || z > height)
		return;
	
	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();

	VertexType& v0 = vertices[z * width + x];
	VertexType& v1 = vertices[(z + 1) * width + x];
	VertexType& v2 = vertices[z * width + x+1];

	Vector3 normal;
	pos.y = interpolateHeight(pos, normal, v0, v1, v2);

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

float Terrain230224::interpolateHeight(Vector3& pos, Vector3& normal, VertexType& v0, VertexType& v1, VertexType& v2)
{
	
	Vector3 v01 = (Vector3)v1.pos - v0.pos;
	Vector3 v02 = (Vector3)v2.pos - v0.pos;
	
	float f1 = pos.x - v0.pos.x / v1.pos.x - v0.pos.x;
	float f2 = pos.z - v0.pos.z / v2.pos.z - v0.pos.z;



	Lerp((Vector3&)v0.normal, (Vector3&)v1.normal, f1);
	Lerp((Vector3&)v0.normal, (Vector3&)v2.normal, f2);



	normal = Cross(v01, v02);
	normal.Normalize();

	float D = -(normal.x * v0.pos.x + normal.y * v0.pos.y + normal.z * v0.pos.z);
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

		//���� ����
		Vector3 e0 = pos1 - pos0;
		Vector3 e1 = pos2 - pos0;

		Vector3 normal = Cross(e0, e1).GetNormalized();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
		//����ȭ�� ���̴����� �ϴ� ����


		//ź��Ʈ
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
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
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
	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			for (int i = 0; i < 6; i++)
				indices.push_back(width * (z + dxz[i].second) + x + dxz[i].first);
		}
	}
}