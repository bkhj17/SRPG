#include "framework.h"
#include "Cylinder.h"

Cylinder::Cylinder(float radius, float height, UINT sliceCount)
	: radius(radius), height(height), sliceCount(sliceCount)
{
	tag = "Cylinder";

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	MakeTangent();
	mesh->CreateMesh();
}

Cylinder::~Cylinder()
{
	delete mesh;
}

void Cylinder::Render()
{
	SetRender();
	mesh->Draw();
}

void Cylinder::MakeMesh()
{
	VertexType vertex = {};
	//Top Circle
	float thetaStep = XM_2PI / sliceCount;

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((size_t)4 * ((size_t)sliceCount + 1));
	
	int topIndex = 0;
	vertex.pos = { 0, height * 0.5f, 0 };
	vertex.uv = { 0.5f, 0.5f };
	vertices.push_back(vertex);

	for (UINT i = 0; i <= sliceCount; i++) {
		float theta = thetaStep * i;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;

		vertex.pos = { x, height * 0.5f, z };
		vertex.uv = { (x + 1) / 2, (z + 1) / 2 };
		vertices.push_back(vertex);
	}

	//bottom
	int bottomIndex = (int)vertices.size();
	vertex.pos = { 0, -height * 0.5f, 0 };
	vertex.uv = { 0.5f, 0.5f };
	vertices.push_back(vertex);

	for (UINT i = 0; i <= sliceCount; i++) {
		float theta = thetaStep * i;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;
		
		vertex.pos = { x, -height * 0.5f, z };
		vertex.uv = { (x+1)/2, (z+1)/2 };
		vertices.push_back(vertex);
	}

	//side
	UINT sideIndex = (size_t)vertices.size();
	for (UINT i = 0; i <= sliceCount; i++) {
		float theta = thetaStep * i;
		float x = cosf(theta) * radius;
		float z = sinf(theta) * radius;

		vertex.pos = { x, height * 0.5f, z };
		vertex.uv = { (float)i / sliceCount, 0.0f};
		vertices.push_back(vertex);

		vertex.pos = { x, -height * 0.5f, z };
		vertex.uv = { (float)i / sliceCount, 1.0f };
		vertices.push_back(vertex);
	}

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve((size_t)sliceCount * 6);
	for (UINT i = 0; i < sliceCount; i++) {
		indices.push_back(0);
		indices.push_back(i+2);
		indices.push_back(i+1);
	}

	for (UINT i = 0; i < sliceCount; i++) {
		indices.push_back(bottomIndex);
		indices.push_back(bottomIndex + i + 1);
		indices.push_back(bottomIndex + i + 2);
	}

	for (UINT i = 0; i < sliceCount; i++) {
		indices.push_back(sideIndex + 2 * i);
		indices.push_back(sideIndex + 2 * i + 2);
		indices.push_back(sideIndex + 2 * i + 1);

		indices.push_back(sideIndex + 2 * i + 1);
		indices.push_back(sideIndex + 2 * i + 2);
		indices.push_back(sideIndex + 2 * i + 3);
	}
}

void Cylinder::MakeNormal()
{

	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();
	for (UINT i = 0; i < indices.size() / 3; i++) {
		UINT index0 = indices[(size_t)i * 3 + 0];
		UINT index1 = indices[(size_t)i * 3 + 1];
		UINT index2 = indices[(size_t)i * 3 + 2];

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
	}
}


void Cylinder::MakeTangent()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (size_t i = 0; i * 3 < indices.size(); i++) {
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
