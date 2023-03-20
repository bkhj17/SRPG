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

cbuffer SelectedBuffer : register(b9)
{
    float4 color;
};

float4 PS(PixelInput input) : SV_TARGET
{
    float u = input.uv.x;
    float v = input.uv.y;
    
    [branch]
    if(u < 0.01 || u > 0.99 || v < 0.01 || v > 0.99)
        return float4(color.xyz, 1.0f);
    else 
        return color;
}