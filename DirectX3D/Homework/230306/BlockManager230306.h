#pragma once

class BlockManager230306 : public Singleton<BlockManager230306>
{
private:
	friend class Singleton;
	BlockManager230306() = default;
	~BlockManager230306();
public:

	void Update();
	void Render();

	void CreateBlocks(UINT width, UINT height);

	float GetHeight(const Vector3& pos) const;

	Cube* BlockMining(const Ray& ray, OUT Contact* hit = nullptr);

	bool IsValidToBuild(Cube* check, Contact& contact);
	bool SetupCube(Cube* check, Texture* texture, Contact& contact);
private:
	list<Cube*> blocks;
};

