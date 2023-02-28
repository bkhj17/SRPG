#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float4 worldPos : WORLD_POS;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormalTangent input)
{
    PixelInput output;
    output.worldPos = mul(input.pos, world);
    output.pos = mul(output.worldPos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    output.tangent = mul(input.tangent, (float3x3) world); // 
    output.binormal = cross(output.normal, output.tangent);
    
    //output.viewDir = normalize(output.pos.xyz - invView._41_42_43);   //
    output.viewDir = normalize(invView._31_32_33); //forward
    
    return output;
}

cbuffer BrushBuffer : register(b10)
{
    int type;
    float3 pickingPos;
    
    float range;
    float3 color;
}

float4 BrushColor(float4 pos)
{
    if (type == 0)
    {
        //원
        float2 direction = pos.xz - pickingPos.xz;
        float distance = length(direction);
        if (distance <= range)
        {
            return float4(color, 0.0f); //더할 것이기에 0
        }
    }
    
    return float4(0, 0, 0, 0);
}

float4 PS(PixelInput input) : SV_TARGET
{
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);
    float3 N = normalize(input.normal);
    
    float3 normal = N;
    float3 light = normalize(lightDirection);
    //float3 light = normalize(input.worldPos - lightDirection);
    if (hasNormalMap)
    {
        float3 normalMapColor = normalMap.Sample(samp, input.uv).rgb;
        normal = normalMapColor * 2.0f - 1.0f;
        float3x3 TBN = float3x3(T, B, N);
        normal = normalize(mul(normal, TBN));
    }
    
    float diffuseIntensity = saturate(dot(normal, -light));

    float4 specular = 0;
    if (diffuseIntensity > 0)
    {
        //Phong Shading : 이론 그대로의 정확한 연산
        //float3 reflection = normalize(reflect(light, normal));  //반사각
        //specular = saturate(dot(-reflection, input.viewDir));
        
        //Blinn Phong Shading(개량형) : 유사한 수치로 효율적으로 연산 가능
        float3 halfWay = normalize(input.viewDir + light);
        specular = saturate(dot(normal, -halfWay));
        
        float4 specularIntensity = specularMap.Sample(samp, input.uv);
                
        specular = pow(specular, shininess) * specularIntensity * mSpecular * lightColor;
    }
    
    float4 diffuse = albedo * diffuseIntensity * mDiffuse * lightColor;
    //간접광
    float4 ambient = albedo * ambientColor * mAmbient;
    
    float4 brushColor = BrushColor(input.worldPos);
    
    return (ambient + diffuse + specular) + brushColor;
}