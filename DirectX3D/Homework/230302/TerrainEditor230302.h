#pragma once

class TerrainEditor230302 : public TerrainEditor
{
private:
	class UseMapBuffer : public ConstBuffer {
	private:
		struct UseMapData {
			int useSecondDiffuse = 0;
			int useSecondNormal = 0;
			int useSecondSpecular = 0;
			int useThirdDiffuse = 0;
			int useThirdNormal = 0;
			int useThirdSpecular = 0;

			int padding[2] = {};
		};
	public:
		UseMapBuffer() : ConstBuffer(&data, sizeof(UseMapData)) {}
		~UseMapBuffer() = default;
	
		UseMapData data;
	};

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
	~TerrainEditor230302();

	void Render();
	void RenderUI();
private:
	void Save(string file);
	void Load(string file);

	HRESULT SaveHeightMapPng(wstring file);
	HRESULT SaveAlphaMapPng(wstring file);
private:
	MapPack mapPack[2] = {};
	UseMapBuffer* useMapBuffer;
};

