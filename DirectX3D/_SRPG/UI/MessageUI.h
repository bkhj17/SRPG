#pragma once

class MessageUI : public UIWindow
{
public:
	MessageUI(wstring file);
	~MessageUI();

	void Render();
private:
	Quad* message;
};

