#pragma once

//정점 : 3차원 공간에서의 한 점

struct VertexUV
{    
    Float3 pos;
    Float2 uv;

    VertexUV() : pos(0, 0, 0), uv(0, 0)
    {
    }

    VertexUV(float x, float y, float z, float u, float v)
        : pos(x, y, z), uv(u, v)
    {     
    }
};

struct Vertex
{
    Float3 pos;

    Vertex()
    {
        pos = { 0.0f, 0.0f, 0.0f };
    }

    Vertex(float x, float y, float z)
    {
        pos = { x, y, z };
    }
};

struct VertexColor
{
    Float3 pos;
    Float4 color;

    VertexColor()
        : pos(0.0f, 0.0f, 0.0f), color(1, 1, 1, 1)
    {
    }

    VertexColor(float x, float y, float z, float r, float g, float b)
        : pos(x, y, z), color(r, g, b, 1)
    {
    }
};