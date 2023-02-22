#pragma once
class Material
{
private:
	const wstring DEFAULT_DIFFUSE_MAP = L"Textures/Color/Magenta.png";

public:
	Material();
	Material(wstring shaderFile);
	~Material();

	void Set();

	void RenderUI();

	void SetShader(wstring shaderFile);
	void SetDiffuseMap(wstring textureFile);
private:
	string name;
	VertexShader* vertexShader;
	PixelShader* pixelShader;

	Texture* diffuseMap = nullptr;
};
