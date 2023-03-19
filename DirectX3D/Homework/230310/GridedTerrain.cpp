#include "framework.h"
#include "GridedTerrain.h"

GridedTerrain::GridedTerrain()
{
	material->SetShader(L"LandScape/Terrain.hlsl");

	widthHeightBuffer = new FloatValueBuffer();
	widthHeightBuffer->Get()[0] = width;
	widthHeightBuffer->Get()[1] = height;
	widthHeightBuffer->Get()[2] = (float)tileWidth;
	widthHeightBuffer->Get()[3] = (float)tileHeight;

	row = width / tileWidth;
	col = height / tileHeight;

	vector<VertexType>& vertices = mesh->GetVertices();
	cubes.resize((size_t)row*col);
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			size_t index = (size_t)i * col + j;
			cubes[index] = new Cube(Vector3(tileWidth, 3.0f, tileHeight));
			cubes[index]->GetMaterial()->SetShader(L"Tile/Tile.hlsl");
			cubes[index]->SetParent(this);

			Vector3 up = Vector3(0, 1, 0);

			Vector3 normal;
			cubes[index]->Pos() = Vector3(tileWidth * j + (tileWidth >> 1), 0, tileHeight * (row - i));
			cubes[index]->Pos().y = GetHeight(cubes[index]->Pos(), &normal);

			cubes[index]->UpdateWorld();
		}
	}

	tileColorBuffer = new ColorBuffer;
	tileColorBuffer->Get() = { 1.0f, 1.0f, 1.0f, 0.5f };
}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;

	for (auto cube : cubes)
		delete cube;
	cubes.clear();
	cubes.shrink_to_fit();
}

void GridedTerrain::Update()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	float dist = FLT_MAX;
	for (int i = 0; i < cubes.size(); i++) {
		Contact contact;
		if (cubes[i]->GetCollider()->IsRayCollision(ray, &contact)) {
			if (contact.distance < dist) {
				dist = contact.distance;
				selected = i;
			}
		}
	}

	UpdateWorld();
}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();

	int index = 0;
	for (auto& cube : cubes) {
		if (index == selected)
			tileColorBuffer->Get() = Float4(0.0f, 0.0f, 1.0f, 0.7f);
		else
			tileColorBuffer->Get() = Float4(1.0f, 1.0f, 1.0f, 0.2f);
		tileColorBuffer->SetPS(9);

		cube->Render();
		index++;
	}
}

Vector3 GridedTerrain::CoordToPos(int x, int y)
{
	return cubes[(size_t)y * col + x]->Pos();
}
