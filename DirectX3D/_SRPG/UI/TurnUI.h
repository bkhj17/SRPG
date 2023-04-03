#pragma once

class TurnUI : public UIWindow
{
public:
	TurnUI(Vector3 pos);
	~TurnUI() = default;

	void Render() override;
};

