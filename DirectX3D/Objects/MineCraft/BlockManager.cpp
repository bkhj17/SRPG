#include "framework.h"

BlockManager::~BlockManager()
{
	for (auto block : blocks)
		delete block;
	blocks.clear();

	blockIndex.clear();
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

	for (int i = 0; i < 5; i++)
		blockIndex[i] = textures[i];	

	for (UINT z = 0; z < height; z++) {
		for (UINT x = 0; x < width; x++) {
			Vector3 pos = { (float)x, 0, (float)z };
			wstring file = textures[Random(0, (int)textures.size())];

			Cube* block = new Cube;
			block->GetMaterial()->SetDiffuseMap(file);
			block->Pos() = pos;
			block->UpdateWorld();

			blocks.push_back(block);
		}
	}
}

float BlockManager::GetHeight(const Vector3& pos) const
{
	Ray ray(pos, Vector3::Down());
	Contact contact;

	float maxHeight = 0.0f;

	for (Cube* block : blocks) {
		if (block->GetCollider()->IsRayCollision(ray, &contact))
			maxHeight = max(contact.hitPoint.y, maxHeight);
	}

	return maxHeight;
}

Cube* BlockManager::GetCollisionBlock()
{
	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));

	float minDistance = FLT_MAX;
	Contact contact;

	list<Cube*>::iterator collisionBlock = blocks.end();
	
	for (auto iter = blocks.begin(); iter != blocks.end(); iter++) {
		auto block = *iter;

		if (block->GetCollider()->IsRayCollision(ray, &contact)) {
			if (contact.distance < minDistance) {
				minDistance = contact.distance;
				collisionBlock = iter;
			}
		}
	}

	if (collisionBlock != blocks.end()) {
		Cube* block = *collisionBlock;
		blocks.erase(collisionBlock);
		return block;
	}
	else
		return nullptr;
}

bool BlockManager::BuildBlock(Cube* block)
{
	if (block == nullptr)
		return false;

	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));
	float minDistance = FLT_MAX;
	Vector3 hitPoint;
	Contact contact;

	Cube* collisionBlock = nullptr;

	for (auto iter = blocks.begin(); iter != blocks.end(); iter++) {
		auto block = *iter;
		if (block->GetCollider()->IsRayCollision(ray, &contact)) {
			if (contact.distance < minDistance) {
				minDistance = contact.distance;
				hitPoint = contact.hitPoint;
				collisionBlock = *iter;
			}
		}
	}

	if (collisionBlock == nullptr)
		return false;

	Vector3 dir = hitPoint - collisionBlock->GlobalPos();
	{
		int maxIndex = 0;
		for (int i = 1; i < 3; i++) {
			if (abs(dir[maxIndex]) < abs(dir[i]))
				maxIndex = i;
		}

		switch (maxIndex)
		{
		case 0:
			dir = { dir.x > 0 ? 1.0f : -1.0f,  0.0f, 0.0f };
			break;
		case 1:
			dir = { 0.0f, dir.y > 0 ? 1.0f : -1.0f, 0.0f };
			break;
		case 2:
			dir = { 0.0f, 0.0f, dir.z > 0 ? 1.0f : -1.0f };
			break;
		}
	}

	block->SetActive(true);
	block->Pos() = collisionBlock->GlobalPos() + dir;
	block->UpdateWorld();
	blocks.push_back(block);
	return true;
}

void BlockManager::Push(Collider* collider)
{
	Cube* collisionBlock = nullptr;

	for (Cube* block : blocks)
	{
		if (block->GetCollider()->IsCollision(collider))
		{
			collisionBlock = block;

			Vector3 dir = collider->GlobalPos() - collisionBlock->GlobalPos();

			int maxIndex = 0;
			for (int i = 0; i < 3; i++)	{
				if (abs(dir[i]) > abs(dir[maxIndex]))
					maxIndex = i;
			}

			if (maxIndex == 1 && dir.y > 0)
				continue;

			switch (maxIndex)
			{
			case 0:
				dir = { dir.x > 0 ? 1.0f : -1.0f,  0.0f, 0.0f };
				break;
			case 1:
				dir = { 0.0f, dir.y > 0 ? 1.0f : -1.0f, 0.0f };
				break;
			case 2:
				dir = { 0.0f, 0.0f, dir.z > 0 ? 1.0f : -1.0f };
				break;
			}

			collider->GetParent()->Pos() += dir * 50.0f * DELTA;
		}
	}
}

int BlockManager::GetIndex(wstring file)
{
	for (auto& bi : blockIndex) {
		if (file == bi.second)
			return bi.first;
	}
	return -1;
}

wstring BlockManager::GetTextureFile(int index)
{
	if(blockIndex.find(index) == blockIndex.end())
		return L"";
	return blockIndex[index];
}

void BlockManager::Save(wstring saveFile)
{
	BinaryWriter* writer = new BinaryWriter(saveFile);
	writer->UInt((UINT)blocks.size());
	for (auto block : blocks) {
		Vector3 pos = block->GlobalPos();
		Vector3 rot = block->Rot();
		Vector3 scale = block->GlobalScale();


		writer->Vector(block->Pos());
		writer->Vector(block->Rot());
		writer->Vector(block->Scale());

		wstring file = block->GetMaterial()->GetDiffuseMap()->GetFile();
		writer->WString(file);
	}
	delete writer;
}

void BlockManager::Load(wstring saveFile)
{
	BinaryReader* reader = new BinaryReader(saveFile);
	if (reader->IsFailed())
		return;

	//리스트라서 애초에 다 지워야 한다. 
	while (!blocks.empty()) {
		delete blocks.back();
		blocks.pop_back();
	}

	UINT cnt = reader->UInt();
	for (UINT i = 0; i < cnt; i++) {
		//blocks
		Vector3 pos = reader->Vector();
		Vector3 rot = reader->Vector();
		Vector3 scale = reader->Vector();
		wstring textureFile = reader->WString();
		
		Cube* block = new Cube;
		block->Pos() = pos;
		block->Rot() = rot;
		block->Scale() = scale;
		block->GetMaterial()->SetDiffuseMap(textureFile);
		blocks.push_back(block);
	}
	delete reader;
}