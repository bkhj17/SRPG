#include "framework.h"
#include "Cube230303.h"

Cube230303::Cube230303(Vector3 size)
{
	mesh = new Mesh<VertexType>;
	MakeMesh(size);
	MakeTangent();
	mesh->CreateMesh();

	collider = new BoxCollider(size);
	collider->SetParent(this);
}

Cube230303::~Cube230303()
{
	delete mesh;
	delete collider;
}

void Cube230303::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void Cube230303::Render()
{
	if (!isActive)
		return;

	SetRender();
	mesh->Draw();
}

void Cube230303::MakeMesh(Vector3 size)
{
	Vector3 halfSize = size * 0.5f;

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
			VertexType vertex;
			Vector3 pos = {
				halfSize.x * points[square[i]].x,
				halfSize.y * points[square[i]].y,
				halfSize.z * points[square[i]].z
			};

			vertex.pos = pos;
			vertex.uv = { uvs[i].x, uvs[i].y };
			vertex.normal = pos.GetNormalized();
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

void Cube230303::MakeTangent()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (int i = 0; i * 3 < indices.size(); i++) {
		int index0 = indices[i * 3];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		Vector3 pos0 = vertices[index0].pos;
		Vector3 pos1 = vertices[index1].pos;
		Vector3 pos2 = vertices[index2].pos;

		Float2 uv0 = vertices[index0].uv;
		Float2 uv1 = vertices[index1].uv;
		Float2 uv2 = vertices[index2].uv;

		Vector3 e0 = pos1 - pos0;
		Vector3 e1 = pos2 - pos0;

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
