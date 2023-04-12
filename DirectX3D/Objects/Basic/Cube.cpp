#include "framework.h"
#include "Cube.h"

Cube::Cube(Vector3 size)
	: size(size)
{
	tag = "Cube";

	mesh = new Mesh<VertexType>;
	MakeMesh();
	mesh->CreateMesh();
	SetDebugName();

	collider = new BoxCollider(size);
	collider->SetParent(this);
}

Cube::~Cube()
{
	delete mesh;
	delete collider;
}

void Cube::Update()
{
	if (!Active())
		return;

	UpdateWorld();
}

void Cube::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void Cube::Render()
{
	if (!Active())
		return;
	
	SetRender();
	mesh->Draw();
}

void Cube::SetDebugName(string name)
{
	if (mesh)
		mesh->SetDebugName(name + ".Cube");
}

void Cube::MakeMesh()
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
			vertices.emplace_back(
				halfSize.x * points[square[i]].x,
				halfSize.y * points[square[i]].y,
				halfSize.z * points[square[i]].z,
				uvs[i].x, uvs[i].y);
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
