#include "framework.h"
#include "Steve.h"
#include "Homework/230306/BlockManager230306.h"

Steve::Steve()
{
	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = { CENTER_X, CENTER_Y, 0 };
	crossHair->UpdateWorld();

	prevMousePos = mousePos;
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

	if (KEY_DOWN(VK_SPACE) && !isJump) {
		velocity = jumpPower;
		isJump = true;
	}

	Vector3 delta = mousePos - prevMousePos;
	prevMousePos = mousePos;

	CAM->Rot().y += delta.x * rotSpeed * DELTA;
	CAM->Rot().x -= delta.y * rotSpeed * DELTA;

	Rot().y = CAM->Rot().y;
}

void Steve::Jump()
{
	velocity -= GRAVITY * DELTA;
	Pos().y += velocity * DELTA;

//	float height = BlockManager::Get()->GetHeight(Pos());
	float height = BlockManager230306::Get()->GetHeight(Pos());

	if (height > Pos().y - Radius())
	{
		Pos().y = height + Radius();
		velocity = 0.0f;

		isJump = false;
	}
}
