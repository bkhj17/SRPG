#pragma once
class Material
{
private:
	const wstring DEFAULT_DIFFUSE_MAP = L"Textures/Color/White.png";
	const wstring DEFAULT_SPECULAR_MAP = L"Textures/Color/White.png";
	const wstring DEFAULT_NORMAL_MAP = L"Textures/Color/White.png";

	enum MapType {
		DIFFUSE, SPECULAR, NORMAL
	};
public:
	class MaterialBuffer : public ConstBuffer {
	public:
		struct Data {
			Float4 diffuse = { 1, 1, 1, 1 };
			Float4 specular = { 1, 1, 1, 1 };
			Float4 ambient = { 1, 1, 1, 1 };
			Float4 emissive = { 0, 0, 0, 1 };
			float shininess = 24.0f;
			int hasNormalMap = 0;
			float padding[2] = {};
		};
	public:
		MaterialBuffer() : ConstBuffer(&data, sizeof(Data)) {}

		Data& Get() { return data; }
	private:
		Data data;
	};

public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Set();

	void RenderUI();

	void SetShader(wstring shaderFile);
	void SetDiffuseMap(wstring textureFile);
	Texture* GetDiffuseMap() { return diffuseMap; }
	void SetSpecularMap(wstring textureFile);
	Texture* GetSpecularMap() { return specularMap; }
	void SetNormalMap(wstring textureFile);
	Texture* GetNormalMap() { return normalMap; }

	void Save(string file);
	void Load(string file);

	string& GetName() { return name; }
	MaterialBuffer::Data& GetData() { return buffer->Get(); }
private:
	void SelectMap(string name, MapType type);
	void UnSelectMap(MapType type);

	void SaveDialog();
	void LoadDialog();
private:
	string name;
	string file;
	string projectPath;
	string editName;

	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap = nullptr;
	Texture* specularMap = nullptr;
	Texture* normalMap = nullptr;

	MaterialBuffer* buffer;
};

