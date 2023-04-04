#include "framework.h"

MessageUI::MessageUI(wstring file)
	: UIWindow(Vector2(), Vector3())
{
	message = new Quad(file);
	message->Pos() = { CENTER_X, CENTER_Y };
	message->UpdateWorld();
}

MessageUI::~MessageUI()
{
	delete message;
}

void MessageUI::Render()
{
	message->Render();
}
