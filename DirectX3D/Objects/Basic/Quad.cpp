#include "Framework.h"

Quad::Quad(Vector2 size)
    : GameObject(L"Basic/Texture.hlsl")
{
    this->size = size;
    endUV = { 1, 1 };

    mesh = new Mesh<VertexUV>;
    MakeMesh();
    mesh->CreateMesh();
}

Quad::Quad(wstring file, Vector2 startUV, Vector2 endUV)
    : GameObject(L"Basic/Texture.hlsl"), startUV(startUV), endUV(endUV)
{
    material->SetDiffuseMap(file);
    size = (endUV - startUV) * material->GetDiffuseMap()->GetSize();
    
    mesh = new Mesh<VertexUV>;
    MakeMesh();
    mesh->CreateMesh();
}

Quad::~Quad()
{    
    delete mesh;
}

void Quad::Render()
{
    SetRender();
    mesh->Draw();
}

void Quad::MakeMesh()
{
    auto& vertices = mesh->GetVertices();
    auto& indices = mesh->GetIndices();

    float left = -size.x * 0.5f;
    float right = +size.x * 0.5f;
    float top = +size.y * 0.5f;
    float bottom = -size.y * 0.5f;

    vertices.emplace_back(left, top, 0.0f, startUV.x, startUV.y);
    vertices.emplace_back(right, top, 0.0f, endUV.x, startUV.y);
    vertices.emplace_back(left, bottom, 0.0f, startUV.x, endUV.y);
    vertices.emplace_back(right, bottom, 0.0f, endUV.x, endUV.y);

    indices.insert(indices.begin(), { 0, 1, 2, 2, 1, 3 });

}

void Quad::UpdateVertices()
{
    float left = -size.x * 0.5f;
    float right = +size.x * 0.5f;
    float top = +size.y * 0.5f;
    float bottom = -size.y * 0.5f;

    auto& vertices = mesh->GetVertices();

    vertices[0] = VertexUV(left, top, 0.0f, startUV.x, startUV.y);
    vertices[1] = VertexUV(right, top, 0.0f, endUV.x, startUV.y);
    vertices[2] = VertexUV(left, bottom, 0.0f, startUV.x, endUV.y);
    vertices[3] = VertexUV(right, bottom, 0.0f, endUV.x, endUV.y);

    mesh->UpdateVertex();
}
