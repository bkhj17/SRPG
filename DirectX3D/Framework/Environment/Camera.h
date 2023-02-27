#pragma once

struct Ray {
    Vector3 pos;
    Vector3 dir;
};

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void RenderUI();
    
    void SetView();

    void SetTarget(Transform* target) { this->target = target; }

    Vector3 ScreenToWorld(Vector3 pos);
    Vector3 WorldToScreen(Vector3 pos);

    Ray ScreenPointToRay(Vector3 screenPoint);

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