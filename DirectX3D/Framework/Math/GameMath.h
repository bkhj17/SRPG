#pragma once

namespace GameMath
{
    enum class Direction
    {
        UP, DOWN, LEFT, RIGHT, NONE
    };

    int Random(const int& min, const int& max);
    float Random(const float& min, const float& max);
    Vector2 Random(const Vector2& min, const Vector2& max);

    float Clamp(const float& min, const float& max, float value);

    float Lerp(const float& start, const float& end, float t);
    Vector2 Lerp(const Vector2& start, const Vector2& end, float t);    
    Vector3 Lerp(const Vector3& start, const Vector3& end, float t);    
    Vector2 SLerp(const Vector2& start, const Vector2& end, float t);    
    /*
    float Cross(const Vector2& vec1, const Vector2& vec2);
    float Dot(const Vector2& vec1, const Vector2& vec2);
    */
    Vector3 Cross(Vector3& vec1, Vector3& vec2);
    float Dot(Vector3& vec1, Vector3& vec2);

    float Distance(const Vector2& vec1, const Vector2& vec2);

    bool NearlyEqaul(const float& s1, const float& s2) { return abs(s1 - s2) < FLT_EPSILON; }
}