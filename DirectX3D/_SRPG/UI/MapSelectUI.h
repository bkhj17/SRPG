#pragma once
#include "UIWindow.h"
class MapSelectUI : public UIWindow
{
public:
	MapSelectUI(string eventKey, Vector2 size = Vector2(), Vector3 pos = Vector3());
	MapSelectUI(string eventKey, float left, float right, float up, float down);
	~MapSelectUI() = default;

	void Render() override;
protected:
	virtual void ActiveFunc() override;
private:
	string eventKey = "";
};
