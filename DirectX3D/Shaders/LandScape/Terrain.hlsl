#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)
{
    LightPixelInput output;
    //float4 height = heightMap.Load(int3(input.pos.x, input.pos.z, 0));
    //input.pos.y = height.x;
    
    output.pos = mul(input.pos, world);
    
    output.viewPos = normalize(invView._31_32_33);
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    output.tangent = mul(input.tangent, (float3x3) world); // 
    output.binormal = cross(output.normal, output.tangent);
        
    return output;
}

Texture2D alphaMap : register(t10); //¾ËÆÄ¸Ê : Æ¯Á¤ ¸Ê°ú ¼¯À» º¸°£¿ë ¸Ê
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

Material GetMaterialToTerrain(LightPixelInput input)
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

float4 PS(LightPixelInput input) : SV_TARGET
{    
    Material material = GetMaterialToTerrain(input);
    GetMaterial(input);
    
    float4 directional = CalcDirectional(material, lights[0]);
    float4 ambient = CalcAmbient(material);
    
    return ambient + directional;
}