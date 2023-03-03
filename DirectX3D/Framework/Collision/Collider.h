#pragma once

//반직선
struct Ray {
    Vector3 pos;
    Vector3 dir;
};

//충돌 접점
struct Contact {
    string tag;

    Vector3 hitPoint;
    float distance;
};

class Collider : public GameObject
{
protected:
    enum Type
    {
        BOX, SPHERE, CAPSULE
    };

    const float PUSH_SPEED = 500.0f;

public:
    Collider();
    ~Collider();

    void Render();
    
    bool IsCollision(Collider* collider);
    
    virtual bool IsRayCollision(IN Ray ray, OUT Contact* contact = nullptr) = 0;

    virtual bool IsBoxCollision(class BoxCollider* collider) = 0;
    virtual bool IsSphereCollision(class SphereCollider* collider) = 0;
    virtual bool IsCapsuleCollision(class CapsuleCollider* collider) = 0;
    
    void SetColor(Float4 color) { material->GetData().diffuse = color; }
    void SetColor(float r, float g, float b) { material->GetData().diffuse = {r, g, b, 1}; }
protected:
    virtual void MakeMesh() = 0;
protected:
    Type type;
    Mesh<Vertex>* mesh;
};