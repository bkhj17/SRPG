#include "framework.h"
#include "Cube.h"

Cube::Cube(Vector3 size)
{
	tag = "Cube";

	vertices.emplace_back(-size.x, -size.y, -size.z, 1.0f, 0.0f, 0.0f); //0
	vertices.emplace_back(-size.x, +size.y, -size.z, 1.0f, 1.0f, 0.0f);	//1
	vertices.emplace_back(+size.x, -size.y, -size.z, 0.0f, 1.0f, 1.0f);	//2
	vertices.emplace_back(+size.x, +size.y, -size.z, 0.0f, 0.0f, 1.0f);	//3
	vertices.emplace_back(-size.x, -size.y, +size.z, 1.0f, 0.0f, 0.0f);	//4
	vertices.emplace_back(-size.x, +size.y, +size.z, 1.0f, 1.0f, 0.0f);	//5
	vertices.emplace_back(+size.x, -size.y, +size.z, 0.0f, 1.0f, 1.0f);	//6
	vertices.emplace_back(+size.x, +size.y, +size.z, 0.0f, 0.0f, 1.0f);	//7

	indices = {
		//Front
		0, 1, 2, 2, 1, 3,
		//Up
		1, 5, 3, 3, 5, 7,
		//Right
		2, 3, 6, 6, 3, 7,
		//Back
		4, 6, 5, 5, 6, 7,
		//Down
		4, 0, 6, 6, 0, 2,
		//Left
		4, 5, 0, 0, 5, 1
	};

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

Cube::~Cube()
{
	delete vertexBuffer;
	delete indexBuffer;
}

void Cube::Update()
{
	UpdateWorld();
}

void Cube::Render()
{
	SetRender();

	vertexBuffer->Set();
	indexBuffer->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}
