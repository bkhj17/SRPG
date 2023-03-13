#include "framework.h"
#include "TerrainScene230313.h"
#include "Terrain230313.h"

TerrainScene230313::TerrainScene230313()
{
	terrain = new Terrain230313();

	Observer::Get()->AddParamEvent("GetHeight", bind(&TerrainScene230313::GetHeightFromTerrain, this, placeholders::_1, false));
	Observer::Get()->AddParamEvent("GetHeightNormal", bind(&TerrainScene230313::GetHeightFromTerrain, this, placeholders::_1, true));
	Observer::Get()->AddParamEvent("GetPickingPos", bind(&TerrainScene230313::ComputePicking, this, placeholders::_1));
}

TerrainScene230313::~TerrainScene230313()
{
	delete terrain;
}

void TerrainScene230313::Update()
{
	terrain->UpdateWorld();
}

void TerrainScene230313::PreRender()
{
}

void TerrainScene230313::Render()
{
	terrain->Render();
}

void TerrainScene230313::PostRender()
{
}

void TerrainScene230313::GUIRender()
{
}

void TerrainScene230313::GetHeightFromTerrain(void* transformPtr, bool updateNormal)
{
	Transform* transform = (Transform*)transformPtr;

	if (updateNormal) {
		Vector3 normal;
		transform->Pos().y = terrain->GetHeight(transform->GlobalPos(), &normal);
		Vector3 rot = GetRotFromNormal(normal);

		transform->Rot() = Lerp(transform->Rot(), rot, DELTA * 10.0f);
	}
	else {
		transform->Pos().y = terrain->GetHeight(transform->GlobalPos());
	}
}

void TerrainScene230313::ComputePicking(void* pick)
{
	Vector3 pickingPos;
	terrain->ComputePicking(pickingPos);
	Vector3* out = (Vector3*)pick;

	*out = pickingPos;
}
