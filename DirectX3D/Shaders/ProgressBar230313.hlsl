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

cbuffer RateBuffer : register(b9)
{
    int max;
    int cur;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float rate = (float) cur / max;
    if(rate < input.uv.r)
        return float4(0, 0, 0, 0);
    
    return diffuseMap.Sample(samp, input.uv);
}