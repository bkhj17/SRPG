#include "framework.h"

GBuffer::GBuffer()
{
	//diffuseRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	//specularRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	//normalRender = new RenderTarget(WIN_WIDTH, WIN_HEIGHT, DXGI_FORMAT_R8G8B8A8_UNORM);
	diffuseRender = new RenderTarget;
	specularRender = new RenderTarget;
	normalRender = new RenderTarget;

	depthStencil = new DepthStencil(WIN_WIDTH, WIN_HEIGHT, true);

	rtvs[0] = diffuseRender;
	rtvs[1] = specularRender;
	rtvs[2] = normalRender;

	srvs[0] = depthStencil->GetSRV();
	srvs[1] = diffuseRender->GetSRV();
	srvs[2] = specularRender->GetSRV();
	srvs[3] = normalRender->GetSRV();

	for (int i = 0; i < 4; i++) {
		quads[i] = new Quad(Vector2(200, 200));
		quads[i]->Pos() = { 100 + (float)i * 200, 100, 0 };
		Texture* texture = Texture::Add(L"G" + to_wstring(i), srvs[i]);
		quads[i]->GetMaterial()->SetDiffuseMap(texture);
		quads[i]->UpdateWorld();
	}
}

GBuffer::~GBuffer()
{
	delete diffuseRender;
	delete specularRender;
	delete normalRender;
	delete depthStencil;

	for (auto quad : quads)
		delete quad;

	for (int i = 0; i < 4; i++)
		SAFE_RELEASE(srvs[i]);
}

void GBuffer::PostRender()
{
	for (Quad* quad : quads)
		quad->Render();
}

void GBuffer::SetSRVs()
{
	for (int i = 0; i < 4; i++) {
		DC->PSSetShaderResources(10 + i, 1, &srvs[i]);
	}
}

void GBuffer::SetMultiRenderTarget()
{
	RenderTarget::SetMulti(rtvs, 3, depthStencil);
}
