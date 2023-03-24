#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 worldPos : POSITION0;
    float4 reflectionPos : POSITION1;
    float4 refractionPos : POSITION2;
};

cbuffer ReflectionBuffer : register(b10)
{
    matrix reflectionView;
}

PixelInput VS(VertexUV input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos;
    output.reflectionPos = mul(input.pos, reflectionView);
    output.reflectionPos = mul(output.reflectionPos, projection);
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    output.refractionPos = output.pos;
    
    return output;
}

Texture2D reflectionMap : register(t10);
Texture2D refractionMap : register(t11);
Texture2D refractionNormalMap : register(t12);

cbuffer WaterBuffer : register(b10)
{
    float4 waveColor;
    
    float waveTime;
    float waveSpeed;
    float waveScale;
    float waveShininess;
    float fresnel;
};

float4 PS(PixelInput input) : SV_TARGET
{
    float4 refPos = input.reflectionPos;
    float2 uv;
    float4 normal = refractionNormalMap.Sample(samp, input.uv) * 2.0f - 1.0f;
    
    uv.x = refPos.x / refPos.w * 0.5f + 0.5f;
    uv.y = -refPos.y / refPos.w * 0.5f + 0.5f;
    uv += normal.xy * waveScale;
    
    float4 reflectionColor = reflectionMap.Sample(samp, uv);
    
    refPos = input.refractionPos;
    uv.x = refPos.x / refPos.w * 0.5f + 0.5f;
    uv.y = -refPos.y / refPos.w * 0.5f + 0.5f;    
    uv += normal.xy * waveScale;
    
    float4 refractionColor = refractionMap.Sample(samp, uv);
    
    float fresnelTerm = 0.02f + 0.97f * pow((1 - dot(invView._31_32_33, normal.xyz)), 5);
    float4 albedo = lerp(reflectionColor, refractionColor, fresnelTerm);

    float3 light = normalize(lights[0].direction);
    light.yz *= -1.0f;
    
    float3 viewDir = normalize(invView._31_32_33);
    float3 halfWay = normalize(viewDir + light);
    float specularIntensity = saturate(dot(-halfWay, normal.xyz));
    
    [flatten]
    if(specularIntensity > 0.0f) {
        //Á¤¹Ý»ç
        specularIntensity = pow(specularIntensity, waveShininess);
        albedo = saturate(albedo + specularIntensity);
    }
    
    return albedo * waveColor;
}