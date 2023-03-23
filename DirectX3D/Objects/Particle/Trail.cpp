#include "framework.h"
#include "Trail.h"

Trail::Trail(wstring imageFile, Transform* start, Transform* end, UINT width, float speed)
	: start(start), end(end), width(width), speed(speed)
{
	material->SetShader(L"Basic/Texture.hlsl");
	material->SetDiffuseMap(imageFile);

	CreateMesh();

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->CullMode(D3D11_CULL_NONE);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additive();
}

Trail::~Trail()
{
	delete mesh;

	delete rasterizerState[0];
	delete rasterizerState[1];
	delete blendState[0];
	delete blendState[1];
}

void Trail::Update()
{
	if (!Active()) return;

	auto& vertices = mesh->GetVertices();

	for (size_t i = 0; i <= width; i++) {
		Vector3 startPos = vertices[i * 2].pos;
		Vector3 endPos = vertices[i * 2 + 1].pos;

		Vector3 startDestPos;
		Vector3 endDestPos;

		if (i == 0) {
			startDestPos = start->GlobalPos();
			endDestPos = end->GlobalPos();
		}
		else {
			startDestPos = vertices[(i - 1) * 2].pos;
			endDestPos = vertices[(i - 1) * 2 + 1].pos;
		}

		startPos = Lerp(startPos, startDestPos, speed * DELTA);
		endPos = Lerp(endPos, endDestPos, speed * DELTA);

		vertices[i * 2].pos = startPos;
		vertices[i * 2 + 1].pos = endPos;
	}

	mesh->UpdateVertex();
}

void Trail::Render()
{
	if (!Active()) return;

	worldBuffer->SetVS(0);
	material->Set();

	rasterizerState[1]->SetState();
	blendState[1]->SetState();

	mesh->Draw();

	rasterizerState[0]->SetState();
	blendState[0]->SetState();
}

void Trail::CreateMesh()
{
	mesh = new Mesh<VertexUV>();

	auto& vertices = mesh->GetVertices();

	vertices.reserve(((size_t)width+1) * 2);
	for (UINT i = 0; i <= width; i++) {
		VertexUV vertex;
		vertex.uv = { (float)i / width, 0.0f };
		vertices.push_back(vertex);

		vertex.uv = { (float)i / width, 1.0f };
		vertices.push_back(vertex);
	}

	vector<UINT>& indices = mesh->GetIndices();
	indices.reserve((size_t)width * 6);
	for (UINT i = 0; i < width; i++) {
		indices.push_back(i * 2);
		indices.push_back(i * 2 + 2);
		indices.push_back(i * 2 + 1);

		indices.push_back(i * 2 + 1);
		indices.push_back(i * 2 + 2);
		indices.push_back(i * 2 + 3);
	}
	mesh->CreateMesh();
}
