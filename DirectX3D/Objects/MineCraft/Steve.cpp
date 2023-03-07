#include "framework.h"
#include "Steve.h"
#include "Homework/230306/BlockManager230306.h"

Steve::Steve()
{
	ClientToScreen(hWnd, &clientCenterPos);
	ShowCursor(false);
	mineUI = new MineUI;
}

Steve::~Steve()
{
	delete mineUI;
}

void Steve::Update()
{
	BlocksSaveLoad();

	Control();
	Jump();
	UpdateWorld();

	mineUI->Update();
}

void Steve::Render()
{
	//__super::Render();
}

void Steve::PostRender()
{
	mineUI->PostRender();
}

void Steve::GUIRender()
{
	mineUI->GUIRender();
}

void Steve::BlocksSaveLoad()
{
	if (KEY_DOWN(VK_F6)) {
		BlockManager::Get()->Save(L"TextData/House.mct");
		mineUI->Save(L"TextData/House.inv");
	}

	if (KEY_DOWN(VK_F7)) {
		BlockManager::Get()->Load(L"TextData/House.mct");
		mineUI->Load(L"TextData/House.inv");
	}
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

	
	Vector3 delta = mousePos - Vector3(CENTER_X, CENTER_Y);
	SetCursorPos(clientCenterPos.x, clientCenterPos.y);

	CAM->Rot().y += delta.x * rotSpeed * DELTA;
	CAM->Rot().x -= delta.y * rotSpeed * DELTA;

	Rot().y = CAM->Rot().y;

	if (!ImGui::GetIO().WantCaptureMouse) {
		if (KEY_DOWN(VK_LBUTTON))
			mineUI->Mining();
		if (KEY_DOWN(VK_RBUTTON))
			mineUI->Build();
	}
}

void Steve::Jump()
{
	velocity -= GRAVITY * DELTA;
	Pos().y += velocity * DELTA;

	float height = BlockManager::Get()->GetHeight(Pos());
//	float height = BlockManager230306::Get()->GetHeight(Pos());	//°úÁ¦¿ë

	if (height > Pos().y - Radius())
	{
		Pos().y = height + Radius();
		velocity = 0.0f;

		isJump = false;
	}
}