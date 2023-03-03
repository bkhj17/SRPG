#pragma once

class Quad : public GameObject
{
public:
    Quad(Vector2 size);
    Quad(wstring file, Vector2 startUV = Vector2(), Vector2 endUV = Vector2(1, 1));
    ~Quad();

    virtual void Render();

    void UpdateVertices();

    Vector2 GetSize() { return size; }
    Vector2 Half() { return size * 0.5f; }

    
private:
    void MakeMesh();

protected:
    Mesh<VertexUV>* mesh;

    Vector2 size;

    Vector2 startUV;
    Vector2 endUV;
};