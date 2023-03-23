#include "Framework.h"

Quad::Quad(Vector2 size)       
    : GameObject(L"Basic/Texture.hlsl"), size(size)
{
    startUV = {};
    endUV = { 1, 1 };

    mesh = new Mesh<VertexType>;
    MakeMesh();
    mesh->CreateMesh();
}

Quad::Quad(wstring file, Vector2 startUV, Vector2 endUV)
    : GameObject(L"Basic/Texture.hlsl"), startUV(startUV), endUV(endUV)
{
    Texture* texture = Texture::Add(file);
    material->SetDiffuseMap(file);
    size = (endUV - startUV) * texture->GetSize();

    mesh = new Mesh<VertexType>;
    MakeMesh();
    mesh->CreateMesh();
}

Quad::~Quad()
{    
    delete mesh;
}

void Quad::Render()
{
    if (!Active())
        return;

    SetRender();
    mesh->Draw();
}

void Quad::SetRender()
{
    mesh->GetVertexBuffer()->Set();
    mesh->GetIndexBuffer()->Set();

    __super::SetRender();
}

void Quad::MakeMesh()
{
    float left = -size.x * 0.5f;
    float right = +size.x * 0.5f;
    float top = +size.y * 0.5f;
    float bottom = -size.y * 0.5f;

    auto& vertices = mesh->GetVertices();
    vertices.emplace_back(left, top, 0.0f, startUV.x, startUV.y);
    vertices.emplace_back(right, top, 0.0f, endUV.x, startUV.y);
    vertices.emplace_back(left, bottom, 0.0f, startUV.x, endUV.y);
    vertices.emplace_back(right, bottom, 0.0f, endUV.x, endUV.y);

    auto& indices = mesh->GetIndices();
    indices = { 0, 1, 2, 2, 1, 3 };
}