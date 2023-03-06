#pragma once

class Quad : public GameObject
{
public:
    Quad(Vector2 size);
    Quad(wstring file, Vector2 startUV = Vector2(), Vector2 endUV = Vector2(1, 1));
    ~Quad();

    virtual void Render();

private:
    void MakeMesh();
protected:
    Vector2 size;
    Vector2 startUV, endUV;

    Mesh<VertexUV>* mesh;
};