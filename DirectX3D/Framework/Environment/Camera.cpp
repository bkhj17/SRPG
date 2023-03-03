#include "Framework.h"

Camera::Camera()
{
    tag = "Camera";

    viewBuffer = new ViewBuffer();
    viewBuffer->SetVS(1);

    Load();

    prevMousePos = mousePos;
}

Camera::~Camera()
{
    delete viewBuffer;

    Save();
}

void Camera::Update()
{
    if (target)
        FollowMode();
    else
        FreeMode();

    if (Rot().x > 360.0f) {
        Rot().x -= 360.0f;
    }
    if (Rot().x < -360.0f)
        Rot().x += 360.0f;
    if (Rot().y > 360.0f)
        Rot().y -= 360.0f;
    if (Rot().y < -360.0f)
        Rot().y += 360.0f;
    SetView();
}

void Camera::SetView()
{
    UpdateWorld();

    view = XMMatrixInverse(nullptr, world);

    viewBuffer->Set(view, world);
    viewBuffer->SetVS(1);
}

Vector3 Camera::ScreenToWorld(Vector3 pos)
{
    return XMVector3TransformCoord(pos, world);
}

Vector3 Camera::WorldToScreen(Vector3 pos)
{
    return XMVector3TransformCoord(pos, view);
}
Ray Camera::ScreenPointToRay(Vector3 screenPoint)
{
    Vector3 screenSize(WIN_WIDTH, WIN_HEIGHT, 1.0f);

    Vector2 point;
    point.x = (screenPoint.x / screenSize.x) * 2.0f - 1.0f;
    point.y = (screenPoint.y / screenSize.y) * 2.0f - 1.0f;

    Matrix projection = Environment::Get()->GetProjection();

    Float4x4 temp;
    XMStoreFloat4x4(&temp, projection);
    screenPoint.x = point.x / temp._11;
    screenPoint.y = point.y / temp._22;
    screenPoint.z = 1.0f;

    screenPoint = XMVector3TransformNormal(screenPoint, world);

    Ray ray;
    ray.pos = Pos();
    ray.dir = screenPoint.GetNormalized();

    return ray;
}
void Camera::RenderUI()
{
    __super::RenderUI();
    if (ImGui::TreeNode("CameraOption")) {
        ImGui::DragFloat("MoveSpeed", &moveSpeed);
        ImGui::DragFloat("RotSpeed", &rotSpeed);

        __super::RenderUI();
        ImGui::TreePop();
    }
}
void Camera::FreeMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    if (KEY_PRESS(VK_RBUTTON))
    {
        if (KEY_PRESS('W')) {
            Pos() += Forward() * moveSpeed * DELTA;
            //Pos().z += moveSpeed * DELTA;
        }
        if (KEY_PRESS('S')) {
            Pos() += Back() * moveSpeed * DELTA;
            //Pos().z -= moveSpeed * DELTA;
        }
        if (KEY_PRESS('A')) {
            Pos() += Left() * moveSpeed *DELTA;
            //Pos().x -= moveSpeed * DELTA;
        }
        if (KEY_PRESS('D')) {
            Pos() += Right() * moveSpeed * DELTA;
            //Pos().x += moveSpeed * DELTA;
        }
        if (KEY_PRESS('Q')) {
            Pos() += Down() * moveSpeed * DELTA;
            //Pos().y += moveSpeed * DELTA;
        }
        if (KEY_PRESS('E')) {
            Pos() += Up() * moveSpeed * DELTA;
            //Pos().y -= moveSpeed * DELTA;
        }
        Rot().x -= delta.y * rotSpeed * DELTA;
        Rot().y += delta.x * rotSpeed * DELTA;
    }
}

void Camera::FollowMode()
{
    Vector3 delta = mousePos - prevMousePos;
    prevMousePos = mousePos;

    Pos() = target->GlobalPos() + arm;

    Rot().x -= delta.y * rotSpeed * DELTA;
    Rot().y += delta.x * rotSpeed * DELTA;
}
