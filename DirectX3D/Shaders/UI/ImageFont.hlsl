#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

//PixelImageFont
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

cbuffer ValueBuffer : register(b1)
{
    int value;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float2 uv = input.uv;
    uv.x += 0.1f * value;
    float4 tex = diffuseMap.Sample(samp, uv);    
    
    return tex;
}