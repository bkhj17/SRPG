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
    //float4 height = heightMap.Load(int3(input.pos.x, input.pos.z, 0));
    //input.pos.y = height.x;
    
    output.pos = mul(input.pos, world);
    
    output.viewPos = invView._41_42_43; //forward
    output.worldPos = output.pos;
    
    output.clipPos = mul(output.pos, lightView);
    output.clipPos = mul(output.clipPos, lightProjection);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    output.tangent = mul(input.tangent, (float3x3) world); // 
    output.binormal = cross(output.normal, output.tangent);
    
    return output;
}

Texture2D depthMap : register(t10);
Texture2D alphaMap : register(t11); //알파맵 : 특정 맵과 섞을 보간용 맵
Texture2D secondMap : register(t12);
Texture2D thirdMap : register(t13);

Material GetMaterialToTerrain(PixelInput input)
{
    Material material;
    material.normal = NormalMapping(input.tangent,
        input.binormal, input.normal, input.uv);
    
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float4 alpha = alphaMap.Sample(samp, input.uv);
    float4 second = secondMap.Sample(samp, input.uv);
    float4 third = thirdMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, alpha.r);
    albedo = lerp(albedo, third, alpha.g);

    material.diffuseColor = albedo;
    
    material.specularIntensity = diffuseMap.Sample(samp, input.uv);
    material.viewPos = input.worldPos.rgb + input.viewPos * 1.0f;
    material.worldPos = input.worldPos;
    
    return material;
}

float4 PS(PixelInput input) : SV_TARGET
{
    Material material = GetMaterialToTerrain(input);
    
    //각 라이트 컬러를 합산
    float4 color = CalcLights(material);
    
    //그림자 처리
    float2 uv = input.clipPos.xy / input.clipPos.w;
    uv.y = -uv.y;
    uv = uv * 0.5f + 0.5f;
    
    if (uv.x < 0.0f || uv.x > 1.0f || uv.y < 0.0f || uv.y > 1.0f)
        return color;
    
    float shadowDepth = depthMap.Sample(samp, uv).r;
    float currentDepth = input.clipPos.z / input.clipPos.w;
    
    if (currentDepth > shadowDepth + 0.0001f)
        return color * 0.5f;
    
    return color;
}