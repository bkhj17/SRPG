#pragma once
class Shadow
{
public:
	Shadow(UINT width = 2000, UINT height = 2000);	//±×¸²ÀÚ Ä÷¸®Æ¼¿¡ ºñ·Ê
	~Shadow();

	void SetRenderTarget();
	void SetRender();
	void PostRender();
	void GUIRender();

private:
	void SetViewProjection();

private:
	UINT width, height;
	float scale = 1.0f;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	ViewBuffer* viewBuffer;
	MatrixBuffer* projectionBuffer;

	class Quad* quad;
};

