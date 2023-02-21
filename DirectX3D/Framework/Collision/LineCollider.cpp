#include "Framework.h"

LineCollider::LineCollider(Vector3 startPos, Vector3 endPos)
    : startPos(startPos), endPos(endPos)
{
    vertices.emplace_back(startPos.x, startPos.y, startPos.z);
    vertices.emplace_back(endPos.x, endPos.y, endPos.z);

    vertexBuffer = new VertexBuffer(vertices.data(),
        sizeof(Vertex), vertices.size());
}
/*
bool LineCollider::IsPointCollision(Vector2 point)
{
    return false;
}

bool LineCollider::IsRectCollision(RectCollider* rect, Vector2* overlap)
{
    return false;
}

bool LineCollider::IsCircleCollision(CircleCollider* circle)
{
    return false;
}

bool LineCollider::PushCollider(Collider* collider)
{
    return false;
}
*/
