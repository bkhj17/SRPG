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

Texture2D map : register(t0);   //테두리

cbuffer ValueBuffer : register(b10)
{
    float2 weight;              //두께 (0~1)
    float value;                //안쪽 투명도
}

float4 PS(PixelInput input) : SV_TARGET
{
    bool isFrame = false;
    float2 uv;
    
    float p1 = 1.0 / 3.0;
    float p2 = 2.0 / 3.0;
    
    if (input.uv.x < weight.x)
    {
        isFrame = true;
        uv.x = input.uv.x / weight.x * p1;
    }
    else if (input.uv.x > 1 - weight.x)
    {
        isFrame = true;
        uv.x = p2 + (input.uv.x - (1 - weight.x)) / weight.x * p1;
    }
    else
        uv.x = p1 + (input.uv.x - weight.x) / (1 - 2 * weight.x) * p1;
    
    if (input.uv.y < weight.y)
    {
        isFrame = true;
        uv.y = input.uv.y / weight.y * p1;
    }
    else if (input.uv.y > 1 - weight.y)
    {
        isFrame = true;
        uv.y = p2 + (input.uv.y - (1 - weight.y)) / weight.y * p1;
    }
    else
        uv.y = p1 + (input.uv.y - weight.y) / (1 - 2 * weight.y) * p1;
    
    
    float4 albedo = map.Sample(samp, uv);
    if(!isFrame)
        albedo.w *= value;
    
    return albedo;
}