
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

cbuffer FrameBuffer : register(b3)
{
    int clip;
    uint curFrame;
}

//모션 여러개 하려면 3차원배열 필요
Texture2DArray transformMap : register(t0);

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

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentAlpha
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 alpha : ALPHA;
};


struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
};

//Vertex Function
matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix cur, next;
    
    float4 c0, c1, c2, c3;
    float4 n0, n1, n2, n3;
    
    for (int i = 0; i < 4; i++)
    {
        //PS 이외에서는 Sample 대신 Load를 쓴다
        c0 = transformMap.Load(int4(indices[i] * 4 + 0, curFrame, clip, 0));
        c1 = transformMap.Load(int4(indices[i] * 4 + 1, curFrame, clip, 0));
        c2 = transformMap.Load(int4(indices[i] * 4 + 2, curFrame, clip, 0));
        c3 = transformMap.Load(int4(indices[i] * 4 + 3, curFrame, clip, 0));
        
        cur = matrix(c0, c1, c2, c3);
        transform += mul(weights[i], cur);
    }
    return transform;
}