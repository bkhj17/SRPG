#pragma once
class GBuffer
{
public:
	GBuffer();
	~GBuffer();

	void PreRender();
	void PostRender();

	void SetMultiRenderTarget();
private:
	RenderTarget* diffuseRender;
	RenderTarget* specularRender;
	RenderTarget* normalRender;

	DepthStencil* depthStencil;
	RenderTarget* rtvs[3];

	class Quad* quads[4];	//테스트용
	ID3D11ShaderResourceView* srvs[4];
};

