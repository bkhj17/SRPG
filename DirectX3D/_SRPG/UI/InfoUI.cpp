#include "framework.h"
#include "InfoUI.h"

InfoUI::InfoUI(Vector3 pos)
	: UIWindow(Vector2(300.0f, 300.0f), pos)
{
}

InfoUI::~InfoUI()
{
}

void InfoUI::Render()
{
	__super::Render();
}
