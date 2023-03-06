#include "VertexHeader.hlsli"
#include "PixelHeader.hlsli"

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

cbuffer SelectBuffer : register(b13)
{
    int selected;
}

float4 PS(PixelInput input) : SV_TARGET
{
    [branch]
    if (selected &&
        (input.uv.x < 0.05f || input.uv.x > 0.95f
        || input.uv.y < 0.05f || input.uv.y > 0.95f))
        return float4(1, 1, 0, 1);
    else
        return diffuseMap.Sample(samp, input.uv);
}