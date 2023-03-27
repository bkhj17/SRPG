#include "framework.h"
#include "FloatingDamage.h"

void FloatingDamage::Spawn(Vector3 pos, int damage)
{
	isActive = true;
	Pos() = CAM->WorldToScreen(pos);
	UpdateWorld();

	startY = Pos().y;
	endY = startY + 200.0f;
	t = 0.0f;
}

void FloatingDamage::Update()
{
	if (!isActive)
		return;

	t += DELTA;

	float y = SLerp(startY, endY, t);
	Pos().y = y;
	UpdateWorld();

	if (t >= 1.0f)
		isActive = false;
}
