#pragma once
class SkyBox : public Sphere
{
public:
	SkyBox(wstring textureFile);
	~SkyBox();

	void Render();

private:
	Texture* cubeMap;
	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];
};

