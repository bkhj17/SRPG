#pragma once

class Camera : public Transform
{
public:
    Camera();
    ~Camera();

    void Update();
    void GUIRender();
    
    void SetView();

    void SetTarget(Transform* target) { this->target = target; }

    Vector3 ScreenToWorld(Vector3 screenPos);
    Vector3 WorldToScreen(Vector3 worldPos);

    struct Ray ScreenPointToRay(Vector3 screenPoint);

    void LookAtTarget();

    void TargetOptionSave(string file);
    void TargetOptionLoad(string file);

    bool ContainPoint(Vector3 point);
    bool ContainSphere(Vector3 center, float radius);

    ViewBuffer* GetViewBuffer() { return viewBuffer; }

    void SetRotX(float rotX) { this->rotX = rotX; }
    void SetRotY(float rotY) { this->rotY = rotY; }
    void SetHeight(float height) { this->height = height; }
    void SetFollowDistance(float distance) { this->distance = distance; }
    void SetFollowImmadiately(bool value) { followImmadiately = value; }
private:
    void FreeMode();
    void FollowMode();

    void Frustum();
private:
    ViewBuffer* viewBuffer;
    Matrix view;
    Matrix projection;

    Vector3 planes[6];
    float a, b, c, d;

    float moveSpeed = 50.0f;
    float rotSpeed = 10.0f;

    Vector3 prevMousePos = {};

    Transform* target = nullptr;

    float distance = 20.0f;
    float height = 20.0f;
    float moveDamping = 5.0f;
    float rotDamping = 1.0f;
    bool followImmadiately = false;

    float rotX = 0.0f, rotY = 0.0f;

    float destRot = 0.0f;
    Vector3 destPos;
    Vector3 focusOffset;

    bool isLookAtTargetX = true;
    bool isLookAtTargetY = true;

    Matrix rotMatrix;

    char file[128] = {};
};