#include "framework.h"
#include "GridedTerrain.h"

GridedTerrain::GridedTerrain()
{
	material->SetShader(L"LandScape/GridedTerrain.hlsl");

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
			cubes[index] = new Cube(Vector3(tileWidth, 1.0f, tileHeight));
			cubes[index]->SetParent(this);

			//그 안의 평면들을 이루는 정점 중 가장 높은 녀석을 필요로 한다
			float mx = 0.0f;
			for (int ii = i * tileHeight; ii <= (i + 1) * tileHeight; ii++) {
				for (int jj = j * tileWidth; jj <= (j + 1) * tileWidth; jj++)
					mx = max(mx, vertices[ii * width + jj].pos.y);
			}
			cubes[index]->Pos() = Vector3(tileWidth * j + (tileWidth>>1), mx, tileHeight * (row - i));
			cubes[index]->UpdateWorld();
		}
	}
}

GridedTerrain::~GridedTerrain()
{
	delete widthHeightBuffer;

	for (auto cube : cubes)
		delete cube;
	cubes.clear();
	cubes.shrink_to_fit();

}

void GridedTerrain::Render()
{
	widthHeightBuffer->SetPS(12);

	__super::Render();
	for (auto& cube : cubes) {
		cube->Render();
	}
}
