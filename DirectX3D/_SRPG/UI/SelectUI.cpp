#include "framework.h"

SelectUI::SelectUI(Vector2 size, Vector3 pos)
	: UIWindow(size, pos)
{
	renderCursor = true;
}

void SelectUI::Update()
{
	if (!Active())
		return;

	__super::Update();

	cQuad->Pos().x = CursorX();
	cQuad->Pos().y = LineY(cursor);
	cQuad->UpdateWorld();
}

void SelectUI::Render()
{
	if (!Active())
		return;

	__super::Render();
	for (int i = 0; i < text.size(); i++)
		Font::Get()->RenderText(text[i], Vector2(GlobalPos().x, GlobalPos().y + LineY(i)));
}

float SelectUI::CursorX()
{
	return -Half().x + 30.0f;
}

float SelectUI::LineY(int lineNum)
{
	return Half().y - 40.0f - lineNum * (cQuad->Half().y + 30.0f);
}
