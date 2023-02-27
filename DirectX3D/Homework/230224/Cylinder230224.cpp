#include "framework.h"
#include "Cylinder230224.h"

Cylinder230224::Cylinder230224(float radius, float height, int sliceCount)
    : radius(radius), height(height), sliceCount(sliceCount)
{
    mesh = new Mesh<VertexType>;
    MakeMesh();
    mesh->CreateMesh();
}

Cylinder230224::~Cylinder230224()
{
    delete mesh;
}

void Cylinder230224::Render()
{
    SetRender();
    mesh->Draw();
}

void Cylinder230224::MakeMesh()
{
    auto& vertices = mesh->GetVertices();
    auto& indices = mesh->GetIndices();

    float dTheta = 2.0f * XM_PI / sliceCount;
    
    //top base
    VertexType vertex;
    vertex.pos = { 0.0f, height * 0.5f, 0.0f };
    vertex.uv = { 0.0f, 0.0f };
    vertex.normal = { 0.0f, 1.0f, 0.0f };
    vertices.push_back(vertex);


    // Cylinder top vertices
    int topStart = vertices.size();
    for (int i = 0; i <= sliceCount; ++i)
    {
        vertex.pos = { radius * cosf(i * dTheta), height * 0.5f, radius * sinf(i * dTheta) };
        vertex.uv = { (float)i / sliceCount, 0.0f };
        vertex.normal = vertex.pos;
        vertices.push_back(vertex);
    }

    // Cylinder bottom vertices
    int bottomStart = vertices.size();
    for (int i = 0; i <= sliceCount; ++i)
    {
        vertex.pos = { radius * cosf(i * dTheta), -height * 0.5f, radius * sinf(i * dTheta) };
        vertex.uv = { (float)i / sliceCount, 1.0f };
        vertex.normal = vertex.pos;
        vertices.push_back(vertex);
    }
    
    //bottom base
    vertex.pos = { 0.0f, -height * 0.5f, 0.0f };
    vertex.uv = { 0.0f, 0.0f };
    vertex.normal = { 0.0f, -1.0f, 0.0f };
    vertices.push_back(vertex);

    // Cylinder indices
    for (int i = 1; i <= sliceCount; ++i)
    {
        indices.push_back(0);
        indices.push_back(i+1);
        indices.push_back(i);
    }
    for (int i = 0; i < sliceCount; ++i)
    {
        indices.push_back(topStart + i);
        indices.push_back(topStart + i + 1);
        indices.push_back(bottomStart + i);

        indices.push_back(bottomStart + i);
        indices.push_back(topStart + i + 1);
        indices.push_back(bottomStart + i + 1);
    }

    for (int i = 1; i <= sliceCount; ++i)
    {
        indices.push_back(vertices.size()-1);
        indices.push_back(vertices.size()-1 - i-1);
        indices.push_back(vertices.size()-1 - i);
    }
}
