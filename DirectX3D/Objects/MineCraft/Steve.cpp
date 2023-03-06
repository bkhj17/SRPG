#include "framework.h"
#include "Steve.h"

Steve::Steve()
{
	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = { CENTER_X, CENTER_Y, 0 };
	crossHair->UpdateWorld();
}

Steve::~Steve()
{
	delete crossHair;
}

void Steve::Update()
{
	Control();
	Jump();
	UpdateWorld();
}

void Steve::Render()
{
	__super::Render();
}

void Steve::PostRender()
{
	crossHair->Render();
}

void Steve::GUIRender()
{
}

void Steve::Control()
{
	if (KEY_PRESS('W'))
		Pos() += Forward() * moveSpeed * DELTA;
	if (KEY_PRESS('S'))
		Pos() += Back() * moveSpeed * DELTA;
	if (KEY_PRESS('D'))
		Pos() += Right() * moveSpeed * DELTA;
	if (KEY_PRESS('A'))
		Pos() += Left() * moveSpeed * DELTA;

	if (KEY_DOWN(VK_SPACE) && !isJump)
		velocity = jumpPower;
}

void Steve::Jump()
{
	velocity -= GRAVITY * DELTA;
	Pos().y += velocity * DELTA;

	float height = BlockManager::Get()->GetHeight(Pos());

	isJump = !(Pos().y - Radius() < height);
	if (!isJump) {
		Pos().y = height + Radius();
		velocity = 0.0f;
	}
}
