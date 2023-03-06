#pragma once

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

    struct Ray ScreenPointToRay(Vector3 screenPoint);

    void TargetOptionSave(string file);
    void TargetOptionLoad(string file);
private:
    void FreeMode();
    void FollowMode();

private:
    ViewBuffer* viewBuffer;
    Matrix view;

    float moveSpeed = 50.0f;
    float rotSpeed = 10.0f;

    Vector3 prevMousePos = {};

    Transform* target = nullptr;

    float distance = 20.0f;
    float height = 20.0f;
    float moveDamping = 5.0f;
    float rotDamping = 1.0f;

    float rotY = 0;

    float destRot = 0.0f;
    Vector3 destPos;
    Vector3 focusOffset;

    bool isLookAtTarget = true;

    Matrix rotMatrix;

    char file[128] = {};
};