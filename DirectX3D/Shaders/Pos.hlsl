cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

cbuffer ViewBuffer : register(b1)
{
    matrix view;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
	
    return output;
}

cbuffer ColorBuffer : register(b0)
{
    float4 color;
}

float4 PS(PixelInput input) : SV_TARGET
{
    return color;
}