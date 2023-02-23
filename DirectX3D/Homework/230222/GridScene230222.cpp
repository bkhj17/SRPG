#include "framework.h"
#include "GridScene230222.h"

GridScene230222::GridScene230222()
	: width(DEFAULT_VALUE), height(DEFAULT_VALUE)
{
	material = new Material(L"Basic/Grid.hlsl");
	CreateMesh();
	worldBuffer = new MatrixBuffer();
}

GridScene230222::~GridScene230222()
{
	delete material;
	delete mesh;
	delete worldBuffer;
}

void GridScene230222::Update()
{
	if (KEY_DOWN(VK_F2))
		renderGrid = !renderGrid;
}

void GridScene230222::PreRender()
{
}

void GridScene230222::Render()
{

	if (renderGrid) {
		worldBuffer->SetVS(0);
		material->Set();
		mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	}
}

void GridScene230222::PostRender()
{
}

void GridScene230222::GUIRender()
{
	if (ImGui::TreeNode("GridOption")) {
		int postWidth = width;
		int postHeight = height;
		ImGui::DragInt("Width", (int*)&width, 2.0f, 0);
		ImGui::DragInt("Height", (int*)&height, 2.0f, 0);

		if (width != postWidth || height != postHeight) {
			//메시 업데이트
			UpdateMesh();
		}

		ImGui::TreePop();
	}
}

void GridScene230222::CreateMesh()
{
	mesh = new Mesh<VertexColor>();
	SetUpVertices(mesh->GetVertices());
	mesh->CreateMesh();
}

void GridScene230222::UpdateMesh()
{
	if (width * height > maxSize) {
		delete mesh;
		CreateMesh();
	}
	else {
		SetUpVertices(mesh->GetVertices());
		mesh->UpdateVertex();
	}
}

void GridScene230222::SetUpVertices(vector<VertexColor>& vertices)
{
	
	maxSize = max(maxSize, width * height);

	int halfW = width / 2;
	int halfH = height / 2;

	vertices.resize(((size_t)width + height + 3) * 2);



	vertices[0] = VertexColor(-(float)max(halfW, halfH), 0, 0, 1, 0, 0);
	vertices[1] = VertexColor(+(float)max(halfW, halfH), 0, 0, 1, 0, 0);

	vertices[2] = VertexColor(0, -(float)max(halfW, halfH), 0, 0, 1, 0);
	vertices[3] = VertexColor(0, +(float)max(halfW, halfH), 0, 0, 1, 0);

	vertices[4] = VertexColor(0, 0, -(float)max(halfW, halfH), 0, 0, 1);
	vertices[5] = VertexColor(0, 0, +(float)max(halfW, halfH), 0, 0, 1);

	int index = 6;
	for (int x = -halfH; x <= halfH; x++) {
		if (x == 0)
			continue;
		vertices[index++] = VertexColor(x, 0, -halfW, 0.5f, 0.5f, 0.5f);
		vertices[index++] = VertexColor(x, 0, +halfW, 0.5f, 0.5f, 0.5f);
	}

	for (int z = -halfW; z <= halfW; z++) {
		if (z == 0)
			continue;
		vertices[index++] = VertexColor(-halfH, 0, z, 0.5f, 0.5f, 0.5f);
		vertices[index++] = VertexColor(+halfH, 0, z, 0.5f, 0.5f, 0.5f);
	}
}

