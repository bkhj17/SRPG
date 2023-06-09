#pragma once

class Transform
{
public:
    Transform();
    virtual ~Transform() = default;

    virtual void UpdateWorld();
    virtual void GUIRender();

    Vector3 Right() const { return right.GetNormalized(); }
    Vector3 Left() const { return right.GetNormalized() * -1.0f; }
    Vector3 Up() const { return up.GetNormalized(); }
    Vector3 Down() const { return up.GetNormalized() * -1.0f; }
    Vector3 Forward() const { return forward.GetNormalized(); }
    Vector3 Back() const { return forward.GetNormalized() * -1.0f; }

    Vector3 GlobalPos() { return globalPosition; }
    Vector3 GlobalScale() { return globalScale; }

    Transform* GetParent();
    Matrix GetWorld() { return world; }

    void SetParent(Transform* transform) { parent = transform; }
    void SetPivot(Vector3 pivot) { this->pivot = pivot; }
    void SetWorld(const Matrix& matrix) { world = matrix; }

    virtual bool Active();
    bool ActiveSelf() { return isActive; }

    void SetActive(bool isActive) { this->isActive = isActive; }

    void SetTag(string tag) { this->tag = tag; }

    Vector3& Pos() { return localPosition; }
    Vector3& Rot() { return localRotation; }
    Vector3& Scale() { return localScale; }

    void Save();
    void Load(string name);
    void Load() { Load(tag); }
protected:
    string tag;

    Vector3 localPosition;
    Vector3 localRotation;
    Vector3 localScale = { 1, 1, 1 };

    bool isActive = true;    

    Matrix world = XMMatrixIdentity();

private:
    Transform* parent = nullptr;

    Vector3 pivot;
    
    Vector3 right, up, forward;
    Vector3 globalPosition, globalScale;

    XMFLOAT4X4 matWorld;
    
};