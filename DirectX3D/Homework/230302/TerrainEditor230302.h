#pragma once

class TerrainEditor230302 : public TerrainEditor
{
private:
	struct MapPack {
		Texture* diffuseMap = nullptr;
		Texture* normalMap = nullptr;
		Texture* specularMap = nullptr;

		void Set(int slot) {
			if (diffuseMap)
				diffuseMap->PSSet(10 + slot);
			if (normalMap)
				normalMap->PSSet(20 + slot);
			if (specularMap)
				specularMap->PSSet(30 + slot);
		}

		void RenderUI();
	};
public:
	TerrainEditor230302();
	~TerrainEditor230302() = default;

	void Render();
	void RenderUI();
private:
	void Save(string file);
	void Load(string file);
private:
	MapPack mapPack[2] = {};
};

