#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewPos : POSITION0;
    float3 worldPos : POSITION1;
    float4 clipPos : POSITION2;
};

cbuffer LightView : register(b11)
{
    matrix lightView;
}

cbuffer LightProjection : register(b12)
{
    matrix lightProjection;
}

PixelInput VS(VertexUVNormalTangentBlend input)
{
    PixelInput output;
    matrix transform;
    
    [branch]
    if (animType)
        transform = mul(SkinWorld(input.indices, input.weights), world);
    else
        transform = world;
        
    output.pos = mul(input.pos, transform);
    
    output.viewPos = invView._41_42_43; //forward
    output.worldPos = output.pos.xyz;
    
    output.clipPos = mul(output.pos, lightView);
    output.clipPos = mul(output.clipPos, lightProjection);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) transform); // 
    output.tangent = mul(input.tangent, (float3x3) transform); // 
    output.binormal = cross(output.normal, output.tangent);
       
    return output;
}

Texture2D depthMap : register(t10);

float4 PS(PixelInput input) : SV_TARGET
{
    Material material;
    material.normal = NormalMapping(input.tangent,
        input.binormal, input.normal, input.uv);
    
    material.diffuseColor = diffuseMap.Sample(samp, input.uv);
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
    
    float4 color = CalcLights(material);
    
    //그림자 처리
    float2 uv = input.clipPos.xy / input.clipPos.w;
    uv.y = -uv.y; //좌표 맞추기
    uv = uv * 0.5f + 0.5f;
    
    if(uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        return color;
    
    float shadowDepth = depthMap.Sample(samp, uv).r;
    float currentDepth = input.clipPos.z / input.clipPos.w;
    
    //이론 상 같은 지점이라면 같은 값이어야 한다
    if (currentDepth > shadowDepth + 0.0001f)
        return color * 0.5f;
    
    return color;
}