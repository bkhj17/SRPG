#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
};

PixelInput VS(VertexInstancing input)
{
    PixelInput output;
    matrix transform;
    [branch]
    if (animType)
        transform = mul(SkinWorld(input.index, input.indices, input.weights), input.transform);
    else
        transform = input.transform;
    
    output.pos = mul(input.pos, transform);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
        
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float depth = input.pos.z / input.pos.w;    //w로 나눈다 == 정규화
    return float4(depth.xxx, 1);
}