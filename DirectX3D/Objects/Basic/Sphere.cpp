#include "framework.h"
#include "Sphere.h"


Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	tag = "Sphere";

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeTangent();
	mesh->CreateMesh();
}

Sphere::~Sphere()
{
	delete mesh;
}


void Sphere::Render()
{
	SetRender();
	mesh->Draw();
}

void Sphere::MakeMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.reserve((stackCount+1) * (sliceCount+1));
	for (UINT i = 0; i <= stackCount; i++) {
		float phi = i * phiStep;
		for (UINT j = 0; j <= sliceCount; j++) {
			float theta = thetaStep * j;

			VertexType vertex;
			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);
			vertex.pos = Vector3(vertex.normal) * radius;
			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;

			vertices.push_back(vertex);
		}
	}


	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve(stackCount * sliceCount * 6);
	for (UINT i = 0; i < stackCount; i++) {
		for (UINT j = 0; j < sliceCount; j++) {
			indices.push_back((sliceCount+1) * i + j);
			indices.push_back((sliceCount+1) * i + (j+1));
			indices.push_back((sliceCount+1) * (i+1) + j);
			indices.push_back((sliceCount+1) * (i+1) + j);
			indices.push_back((sliceCount+1) * i + (j+1));
			indices.push_back((sliceCount+1) * (i+1) + (j+1));
		}
	}
}

void Sphere::MakeTangent()
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

	/*
	for (VertexType& vertex : vertices) {
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		vertex.tangent = (t - n * Dot(n, t)).GetNormalized();
	}
	*/
}
