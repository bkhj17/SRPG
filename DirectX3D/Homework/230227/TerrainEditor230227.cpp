#include "framework.h"
#include "TerrainEditor230227.h"

void TerrainEditor230227::Update()
{
	auto& vertices = mesh->GetVertices();

	bool edit = false;

	if (KEY_PRESS('Z')) {
		pickingPos = Picking();
		UINT pickingPoint = width * (height - 1 - (UINT)pickingPos.z) + (UINT)pickingPos.x;
		vertices[pickingPoint].pos.y += 10.0f * DELTA;
		edit = true;
	}
	else if(KEY_PRESS('X')) {
		pickingPos = Picking();
		UINT pickingPoint = width * (height - 1 - (UINT)pickingPos.z) + (UINT)pickingPos.x;
		vertices[pickingPoint].pos.y -= 10.0f * DELTA;
		edit = true;
	}

	if (edit) {
		MakeNormal();
		MakeTangent();
		mesh->UpdateVertex();
	}

	UpdateWorld();
}
