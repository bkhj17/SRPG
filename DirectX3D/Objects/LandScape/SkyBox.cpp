#include "framework.h"
#include "SkyBox.h"

SkyBox::SkyBox(wstring textureFile)
{
	material->SetShader(L"LandScape/Skybox.hlsl");
	cubeMap = Texture::Add(textureFile);

	rasterizerState[0] = new RasterizerState();
	rasterizerState[1] = new RasterizerState();
	rasterizerState[1]->FrontCounterClockwise(true);

	depthStencilState[0] = new DepthStencilState();
	depthStencilState[1] = new DepthStencilState();
	depthStencilState[1]->DepthEnable(false);
}

SkyBox::~SkyBox()
{
	delete rasterizerState[0];
	delete rasterizerState[1];
	delete depthStencilState[0];
	delete depthStencilState[1];
}

void SkyBox::Render()
{
	rasterizerState[1]->SetState();
	depthStencilState[1]->SetState();

	cubeMap->PSSet(10);
	__super::Render();

	rasterizerState[0]->SetState();
	depthStencilState[0]->SetState();
}
