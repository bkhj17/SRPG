#include "framework.h"
#include "MapSelectUI.h"

MapSelectUI::MapSelectUI(string eventKey, Vector2 size, Vector3 pos)
	: UIWindow(size, pos), eventKey(eventKey)
{
	mapControl = true;
}

MapSelectUI::MapSelectUI(string eventKey, float left, float right, float up, float down)
	: UIWindow(left, right, up, down), eventKey(eventKey)
{
	mapControl = true;
}

void MapSelectUI::Render()
{
}

void MapSelectUI::ActiveFunc()
{
	Observer::Get()->ExcuteEvent(eventKey);
}
