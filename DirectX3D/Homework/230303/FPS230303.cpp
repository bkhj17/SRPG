#include "framework.h"
#include "FPS230303.h"

FPS230303::FPS230303()
{
	collider = new BoxCollider(Vector3(1, 2, 1));
	collider->SetParent(this);
}

FPS230303::~FPS230303()
{
	delete collider;
}

void FPS230303::Update()
{
    Vector3 mv = {};
    if (KEY_PRESS('W'))
        mv = CAM->Forward();
    if (KEY_PRESS('S'))
        mv = CAM->Back();
    if (KEY_PRESS('A'))
        mv = CAM->Left();
    if (KEY_PRESS('D'))
        mv = CAM->Right();

    mv.y = 0.0f;
    Pos() += mv * moveSpeed * DELTA;

    jumpValue -= DELTA * 9.8f;
    if (KEY_PRESS(VK_SPACE)) {
        jumpValue = 10.0f;
    }

    Pos().y += jumpValue * DELTA;

	UpdateWorld();
}

void FPS230303::UpdateWorld()
{
	__super::UpdateWorld();
	collider->UpdateWorld();
}

void FPS230303::StandOn(BoxCollider* collider)
{
    if (!collider->Active())
        return;

    Ray rDownFps;
    rDownFps.dir = Vector3(0, -1, 0);
    rDownFps.pos = GlobalPos();
    Contact cFps;

    if (collider->IsRayCollision(rDownFps, &cFps)) {
        if (cFps.distance > 0.0f && cFps.distance < GlobalScale().y) {
            Pos().y += GlobalScale().y - cFps.distance;
            jumpValue = 0.0f;
            UpdateWorld();
        }
    }
}
