#include "framework.h"
#include "TerrainScene230308.h"

TerrainScene230308::TerrainScene230308()
{
	terrain = new Terrain();

	Observer::Get()->AddParamEvent("GetHeight", bind(&TerrainScene230308::GetHeightFromTerrain, this, placeholders::_1, false));
	Observer::Get()->AddParamEvent("GetHeightNormal", bind(&TerrainScene230308::GetHeightFromTerrain, this, placeholders::_1, true));
}

TerrainScene230308::~TerrainScene230308()
{
	delete terrain;
}

void TerrainScene230308::Update()
{
	terrain->UpdateWorld();
}

void TerrainScene230308::PreRender()
{
}

void TerrainScene230308::Render()
{
	terrain->Render();
}

void TerrainScene230308::PostRender()
{
}

void TerrainScene230308::GUIRender()
{
	terrain->RenderUI();
}

void TerrainScene230308::GetHeightFromTerrain(void* transformPtr, bool updateNormal)
{
	Transform* transform = (Transform*)transformPtr;
	
	if (updateNormal) {
		Vector3 normal;
		transform->Pos().y = terrain->GetHeight(transform->GlobalPos(), &normal);
		transform->Rot() = GetRotFromNormal(normal);
	}
	else {
		transform->Pos().y = terrain->GetHeight(transform->GlobalPos());
	}
}
