#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void RenderUI();
    
    void SetView();

    void SetTarget(Transform* target, Vector3 pivot = {}) { this->target = target; this->arm = pivot; }

    Vector3 ScreenToWorld(Vector3 pos);
    Vector3 WorldToScreen(Vector3 pos);

    struct Ray ScreenPointToRay(Vector3 screenPoint);

private:
    void FreeMode();
    void FollowMode();

private:
    ViewBuffer* viewBuffer;
    Matrix view;

    float moveSpeed = 100.0f;
    float rotSpeed = 10.0f;

    Vector3 prevMousePos = {};

    Transform* target = nullptr;
    Vector3 arm = {};
};