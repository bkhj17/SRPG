#include "framework.h"
#include "GridScene.h"

GridScene::GridScene()
	: curWidth(MAX_VALUE), curHeight(MAX_VALUE), fixWidth(MAX_VALUE), fixHeight(MAX_VALUE)
{
	material = new Material(L"Basic/Grid.hlsl");

	mesh = new Mesh<VertexColor>();
	MakeMesh();
	mesh->CreateMesh();

	worldBuffer = new MatrixBuffer();
}

GridScene::~GridScene()
{
	delete material;
	delete mesh;
	delete worldBuffer;
}

void GridScene::Update()
{
	bool check = false;
	check |= (curWidth != fixWidth || curHeight != fixHeight);
	check |= curGridColor != fixGridColor;

	if (check) return;

	fixWidth = curWidth;
	fixHeight = curHeight;

	fixGridColor = curGridColor;

	if (curWidth != fixWidth || curHeight != fixHeight) {
		MakeMesh();
		mesh->UpdateVertex();
	}
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	worldBuffer->SetVS(0);
	material->Set();
	mesh->Draw(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

void GridScene::PostRender()
{
}

void GridScene::GUIRender()
{
	if (ImGui::TreeNode("GridOption")) {
		ImGui::DragInt("Width", (int*)&curWidth, 1.0f, 0, MAX_VALUE);
		ImGui::DragInt("Height", (int*)&curHeight, 1.0f, 0, MAX_VALUE);

		ImGui::TreePop();
	}
}

void GridScene::MakeMesh()
{
	int halfW = curWidth / 2;
	int halfH = curHeight / 2;

	vector<VertexColor>& vertices = mesh->GetVertices();
	vertices.clear();

	for (int x = -halfH; x <= halfH; x++) {
		if (x == 0)
			continue;
		vertices.emplace_back(x, 0, -halfW, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(x, 0, +halfW, 0.5f, 0.5f, 0.5f);
	}

	for (int z = -halfW; z <= halfW; z++) {
		if (z == 0)
			continue;
		vertices.emplace_back(-halfH, 0, z, 0.5f, 0.5f, 0.5f);
		vertices.emplace_back(+halfH, 0, z, 0.5f, 0.5f, 0.5f);
	}

	vertices.emplace_back(-MAX_VALUE, 0, 0, 1, 0, 0);
	vertices.emplace_back(+MAX_VALUE, 0, 0, 1, 0, 0);

	vertices.emplace_back(0, -MAX_VALUE, 0, 0, 1, 0);
	vertices.emplace_back(0, +MAX_VALUE, 0, 0, 1, 0);

	vertices.emplace_back(0, 0, -MAX_VALUE, 0, 0, 1);
	vertices.emplace_back(0, 0, +MAX_VALUE, 0, 0, 1);
}
