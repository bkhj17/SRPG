#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void SetView();

    void SetTarget(Transform* target) { this->target = target; }

    Vector3 ScreenToWorld(Vector3 pos);
    Vector3 WorldToScreen(Vector3 pos);

    void RenderUI();
private:
    void FreeMode();
    void FollowMode();

private:
    ViewBuffer* viewBuffer;
    Matrix view;

    float moveSpeed = 100.0f;
    float rotSpeed = 10.0f;

    Transform* target = nullptr;
};