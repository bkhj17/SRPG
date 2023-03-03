#include "framework.h"

SphereCollider::SphereCollider()
{
    MakeMesh();
    mesh->CreateMesh();
}

bool SphereCollider::IsRayCollision(IN Ray ray, OUT Contact* contact)
{
    return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
    return false;
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
    return false;
}

bool SphereCollider::IsCapsuleCollision(CapsuleCollider* collider)
{
    return false;
}

void SphereCollider::MakeMesh()
{
}
