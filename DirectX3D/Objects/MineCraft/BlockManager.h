#pragma once

class BlockManager : public Singleton<BlockManager>
{
private:
	friend class Singleton;
	BlockManager() = default;
	~BlockManager();
public:

	void Update();
	void Render();

	void CreateBlocks(UINT width, UINT height);

	float GetHeight(const Vector3& pos) const;

	Cube* GetCollisionBlock();
	bool BuildBlock(Cube* block);

	void Push(Collider* collider);

	int GetIndex(wstring file);
	wstring GetTextureFile(int index);

	void Save(wstring saveFile);
	void Load(wstring saveFile);

private:
	map<int, wstring> blockIndex;
	list<Cube*> blocks;
};