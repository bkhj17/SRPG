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
    float4 alpha : ALPHA;
};

PixelInput VS(VertexUVNormalTangentAlpha input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
	
    output.viewPos = invView._41_42_43;
    output.worldPos = output.pos.xyz;
	
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    output.tangent = mul(input.tangent, (float3x3) world); // 
    output.binormal = cross(output.normal, output.tangent);
        
    output.alpha = input.alpha;
    
    return output;
}

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
    
    float range;
    float3 color;
}

float4 BrushColor(float3 pos)
{
    if (type == 0 || type == 1)
    {
        //원
        float2 direction = pos.xz - pickingPos.xz;
        float distance = length(direction);
        if (distance <= range)
            return float4(color, 0.0f); //더할 것이기에 0
    }
    else if (type == 2)
    {
        //사각형
        float size = range * 0.5f;
        if (abs(pos.x - pickingPos.x) < size && abs(pos.z - pickingPos.z) < size)
            return float4(color, 0.0f);
    }
    
    return float4(0, 0, 0, 0);
}

cbuffer UseMapBuffer : register(b11)
{
    int useSecondDiffuse;
    int useSecondNormal;
    int useSecondSpecular;
    int useThirdDiffuse;
    int useThirdNormal;
    int useThirdSpecular;
}

Texture2D secondDiffuseMap : register(t11);
Texture2D thirdDiffuseMap : register(t12);

Texture2D secondNormalMap : register(t21);
Texture2D thirdNormalMap : register(t22);

Texture2D secondSpecularMap : register(t31);
Texture2D thirdSpeculerMap : register(t32);

Material GetMaterialToTerrain(PixelInput input)
{
    
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float4 alpha = input.alpha;
    float4 second = secondDiffuseMap.Sample(samp, input.uv);
    float4 third = thirdDiffuseMap.Sample(samp, input.uv);
    
    albedo = lerp(albedo, second, alpha.r);
    albedo = lerp(albedo, third, alpha.g);
    
    Material material;
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    if (hasNormalMap)
    {    
        material.normal = normalMap.Sample(samp, input.uv).rgb;
        if(useSecondNormal)
            material.normal = lerp(material.normal, secondNormalMap.Sample(samp, input.uv).rgb * 2.0f - 1.0f, input.alpha.r);
        if(useThirdNormal)        
            material.normal = lerp(material.normal, thirdNormalMap.Sample(samp, input.uv).rgb * 2.0f - 1.0f, input.alpha.g);
        
        float3x3 TBN = float3x3(T, B, N);
        material.normal = normalize(mul(material.normal, TBN));
    }
    else
        material.normal = NormalMapping(input.tangent, input.binormal, input.normal, input.uv);
    
    material.diffuseColor = albedo;
    
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    if(useSecondSpecular)
        material.specularIntensity = lerp(material.specularIntensity, secondSpecularMap.Sample(samp, input.uv), input.alpha.r);
    if(useThirdSpecular)
        material.specularIntensity = lerp(material.specularIntensity, thirdSpeculerMap.Sample(samp, input.uv), input.alpha.g);
    
    material.viewPos = input.viewPos;
    material.worldPos = input.worldPos;
    
    return material;
}

float4 PS(PixelInput input) : SV_TARGET
{
    Material material = GetMaterialToTerrain(input);
    
    float4 color = CalcDirectional(material, lights[0]);
    float4 ambient = CalcAmbient(material);
    float4 emissive = CalcEmissive(material);
    
    return color + ambient + emissive + BrushColor(material.worldPos.xyz);
}