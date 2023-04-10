#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
};

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.uv = input.uv;
    return output;
}

cbuffer TileDataBuffer : register(b8)
{
    float4 teamColor;
    int onCharacter;
}

cbuffer SelectedBuffer : register(b9)
{
    float4 color;
};

float4 PS(PixelInput input) : SV_TARGET
{
    
    float edgeWeight = 0.01f;
    float4 edgeColor = float4(color.xyz, 1.0f);
    if (onCharacter)
    {
        edgeWeight = 0.1f;
        edgeColor = teamColor;
    }
    
    float u = input.uv.x;
    float v = input.uv.y;
           
    [branch]
    if (u < edgeWeight || u > 1 - edgeWeight || v < edgeWeight || v > 1 - edgeWeight)
    {        
        return edgeColor;
    }
    else
        return color;
    
}