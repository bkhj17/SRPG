#pragma once

class SelectUI : public UIWindow
{
public:
	SelectUI(Vector2 size, Vector3 pos = Vector3());

	virtual void Update() override;
	virtual void Render() override;
protected:
	virtual float CursorX();
	virtual float LineY(int lineNum);
protected:
	vector<string> text;
};

