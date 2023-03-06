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
private:
	list<Cube*> blocks;
};

