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

Texture2D backImage : register(t10);

cbuffer ValueBuffer : register(b10)
{
    float fillAMount;
}

float4 PS(PixelInput input) : SV_TARGET
{
    [branch]
    if(input.uv.x < fillAMount)
        return diffuseMap.Sample(samp, input.uv);
    else
        return backImage.Sample(samp, input.uv);
}