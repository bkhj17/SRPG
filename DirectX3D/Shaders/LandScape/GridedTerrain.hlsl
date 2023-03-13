#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
    float3 worldPos : POSITION;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    output.viewDir = normalize(invView._31_32_33);
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world);    // 
    output.tangent = mul(input.tangent, (float3x3) world);  // 
    output.binormal = cross(output.normal, output.tangent);
        
    return output;
}

Texture2D alphaMap : register(t10); //¾ËÆÄ¸Ê : Æ¯Á¤ ¸Ê°ú ¼¯À» º¸°£¿ë ¸Ê
Texture2D secondMap : register(t11);
Texture2D thirdMap : register(t12);

cbuffer WidthHeightUVBuffer : register(b12)
{
    float mapWidth;
    float mapHeight;
    float tileWidth;
    float tileHeight;
};

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
    material.viewPos = input.worldPos.rgb + input.viewDir * 1.0f;
    material.worldPos = input.worldPos;
    
    return material;
}

float4 PS(PixelInput input) : SV_TARGET
{
    float tickness = 0.01;
    
    if (tileWidth > 0.0f)
    {    
        float gridU = input.uv.x * mapWidth;
        while (gridU > tileWidth)
            gridU -= tileWidth;
        gridU /= tileWidth;
        if (gridU < tickness || gridU > 1-tickness)
            return float4(0, 0, 0, 1);
    }
    if (tileHeight > 0.0f)
    {
        float gridV = input.uv.y * mapHeight;
        while (gridV > tileHeight)
            gridV -= tileHeight;
        gridV /= tileHeight;
        if (gridV < tickness || gridV > 1 - tickness)
            return float4(0, 0, 0, 1);
    }
    
    {
        Material material = GetMaterialToTerrain(input);
    
        float4 directional = CalcDirectional(material, lights[0]);
        float4 ambient = CalcAmbient(material);
        float4 emissive = CalcEmissive(material);
    
        return ambient + directional + emissive;
    }
}