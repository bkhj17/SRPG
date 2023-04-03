#include "framework.h"
#include "TurnChangeUI.h"

TurnChangeUI::TurnChangeUI()
	: UIWindow(0, WIN_WIDTH, WIN_HEIGHT, 0)
{
	bg = new Quad(L"Textures/UI/TurnChange.png", Vector2(0.0f, 0.0f), Vector2(1, 1.0f / 3.0f));
	bg->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	bg->SetParent(this);
	bg->UpdateWorld();

	text[0] = new Quad(L"Textures/UI/TurnChange.png", Vector2(0.0f, 1.0f / 3.0f), Vector2(0.55f, 2.0f / 3.0f));
	text[0]->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	text[0]->SetParent(this);
	text[0]->UpdateWorld();

	text[1] = new Quad(L"Textures/UI/TurnChange.png", Vector2(0.0f, 2.0f / 3.0f), Vector2(0.55f, 1.0f));
	text[1]->GetMaterial()->SetShader(L"Basic/Texture.hlsl");
	text[1]->SetParent(this);
	text[1]->UpdateWorld();
}

TurnChangeUI::~TurnChangeUI()
{
	delete bg;
	delete text[0];
	delete text[1];
}

void TurnChangeUI::Init()
{
	isActive = true;
	time = rate;
}

void TurnChangeUI::Update()
{
	time -= DELTA;
	if (time <= 0.0f)
		Close();
}

void TurnChangeUI::Render()
{
	bg->Render();
	text[TurnManager::Get()->GetCurPlayer()]->Render();
}