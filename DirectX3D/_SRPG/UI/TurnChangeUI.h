#pragma once

class TurnChangeUI : public UIWindow
{
private:
	const float DEFAULT_RATE = 2.0f;

public:
	TurnChangeUI();
	~TurnChangeUI();

	void Init();

	void Update() override;
	void Render() override;

private:
	float rate = DEFAULT_RATE;
	float time = 0.0f;

	Quad* bg;
	Quad* text[2] = { nullptr, nullptr };
};

