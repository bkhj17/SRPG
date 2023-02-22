#include "framework.h"
#include "Grid230221.h"

Grid230221::Grid230221()
{
	int maxGrid = 110;
	grids.resize(maxGrid * 6);	
	for (int i = 0; i < maxGrid; i++) {
		grids[i] = new LineCollider(Vector3(-1000.0f, 0.0f, (i - maxGrid / 2) * 10.0f), Vector3(1000.0f, 0.0f, (i - maxGrid / 2) * 10.0f));
		grids[i]->GetColor() = { 1.0f, 0.0f, 0.0f, 1.0f };
		grids[i + maxGrid] = new LineCollider(Vector3((i - maxGrid / 2) * 10.0f, 0.0f, -1000.0f), Vector3((i - maxGrid / 2) * 10.0f, 0.0f, 1000.0f));
		grids[i + maxGrid]->GetColor() = { 1.0f, 0.0f, 0.0f, 1.0f };

		grids[i + maxGrid * 2] = new LineCollider(Vector3(0.0f, -1000.0f, (i - maxGrid / 2) * 10.0f), Vector3(0.0f, 1000.0f, (i - maxGrid / 2) * 10.0f));
		grids[i + maxGrid * 2]->GetColor() = { 0.0f, 1.0f, 0.0f, 1.0f };
		grids[i + maxGrid * 3] = new LineCollider(Vector3(0.0f, (i - maxGrid / 2) * 10.0f, -1000.0f), Vector3(0.0f, (i - maxGrid / 2) * 10.0f, 1000.0f));
		grids[i + maxGrid * 3]->GetColor() = { 0.0f, 1.0f, 0.0f, 1.0f };

		grids[i + maxGrid * 4] = new LineCollider(Vector3((i - maxGrid / 2) * 10.0f, -1000.0f, 0.0f), Vector3((i - maxGrid / 2) * 10.0f, 1000.0f, 0.0f));
		grids[i + maxGrid * 4]->GetColor() = { 0.0f, 0.0f, 1.0f, 1.0f };
		grids[i + maxGrid * 5] = new LineCollider(Vector3(-1000.0f, (i - maxGrid / 2) * 10.0f, 0.0f), Vector3(1000.0f, (i - maxGrid / 2) * 10.0f, 0.0f));
		grids[i + maxGrid * 5]->GetColor() = { 0.0f, 0.0f, 1.0f, 1.0f };
	}
}

Grid230221::~Grid230221()
{
	for (auto g : grids)
		if (g) delete g;
}

void Grid230221::Render()
{
	for (auto g : grids)
		if (g) g->Render();
}
