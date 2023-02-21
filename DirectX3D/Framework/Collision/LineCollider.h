#pragma once

class LineCollider : public Collider
{
public:
    LineCollider(Vector3 startPos, Vector3 endPos);
    ~LineCollider() = default;
    /*
    virtual bool IsPointCollision(Vector3 point) override;
    virtual bool IsRectCollision(RectCollider* rect, Vector2* overlap) override;
    virtual bool IsCircleCollision(CircleCollider* circle) override;
    virtual bool PushCollider(Collider* collider) override;
    */
private:
    Vector3 startPos;
    Vector3 endPos;
};