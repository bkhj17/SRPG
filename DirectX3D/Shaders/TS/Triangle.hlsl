#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

Vertex VS(Vertex input)
{
    return input;
}

struct CHullOutput
{
    float edgeTessFactor[3] : SV_TessFactor;
    float insideTessFactor : SV_InsideTessFactor;
};

#define NUM_CONTROL_POINTS 3

cbuffer EdgeInfo : register(b10)
{
    float3 edges;
    float inside;
}

CHullOutput CHS(InputPatch<Vertex, NUM_CONTROL_POINTS> input)
{
    CHullOutput output;

    output.edgeTessFactor[0] = edges.x;
    output.edgeTessFactor[1] = edges.y;
    output.edgeTessFactor[2] = edges.z;
    output.insideTessFactor = inside;

    return output;
}

struct HullOutput
{
    float4 pos : POSITION;
};

[domain("tri")]                             //삼각형
[partitioning("integer")]                   //정수
[outputtopology("triangle_cw")]             //삼각형-시계방향
[outputcontrolpoints(3)]                    //개수
[patchconstantfunc("CHS")]                  
HullOutput HS(
	InputPatch<Vertex, NUM_CONTROL_POINTS> input,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    HullOutput output;

    output.pos = input[i].pos;

    return output;
}

struct PixelInput
{
    float4 pos : SV_POSITION;
};

[domain("tri")]
PixelInput DS(CHullOutput input, float3 uvw : SV_DomainLocation,
	const OutputPatch<HullOutput, NUM_CONTROL_POINTS> patch)
{
    PixelInput output;

    float4 pos = patch[0].pos * uvw.x + patch[1].pos * uvw.y + patch[2].pos * uvw.z;
    output.pos = float4(pos.xyz, 1.0f);
    
    return output;
}

float4 PS(Vertex input) : SV_TARGET
{
    return float4(1, 1, 0, 1);
}