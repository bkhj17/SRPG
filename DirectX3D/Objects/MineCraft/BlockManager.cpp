#include "framework.h"

BlockManager::~BlockManager()
{
	for (auto block : blocks)
		delete block;
	blocks.clear();
}

void BlockManager::Update()
{
	for (auto block : blocks)
		block->Update();
}

void BlockManager::Render()
{
	for (auto block : blocks)
		block->Render();
}

void BlockManager::CreateBlocks(UINT width, UINT height)
{
	vector<wstring> textures = {
		L"Textures/Block/Dirt.png",
		L"Textures/Block/Sand.png",
		L"Textures/Block/Stone.png",
		L"Textures/Block/Ice.png",
		L"Textures/Block/Gold.png",
	};

	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			Vector3 pos = { (float)x, 0, (float)z };
			wstring file = textures[Random(0, textures.size())];

			Cube* cube = new Cube;
			cube->GetMaterial()->SetDiffuseMap(file);
			cube->Pos() = pos;
			cube->UpdateWorld();

			blocks.push_back(cube);
		}
	}
}

float BlockManager::GetHeight(const Vector3& pos) const
{
	Ray ray(pos, Vector3::Down());
	Contact contact;
	for (Cube* block : blocks) {
		if (block->GetCollider()->IsRayCollision(ray, &contact))
			return contact.hitPoint.y;
	}

	return 0.0f;
}
