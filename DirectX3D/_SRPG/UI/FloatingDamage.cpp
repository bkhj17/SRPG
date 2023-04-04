#include "framework.h"

FloatingDamage::FloatingDamage(wstring file)
	: ImageFont(file) 
{
	isActive = false; 
}

void FloatingDamage::Spawn(Vector3 pos, int damage)
{
	isActive = true;
	SetValue(damage);
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


	t += DELTA * speed;

	float y = SLerp(startY, endY, t);
	Pos().y = y;

	__super::Update();
	if (t >= 1.0f)
		isActive = false;
}
