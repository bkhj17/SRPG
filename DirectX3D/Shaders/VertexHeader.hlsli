
cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};
