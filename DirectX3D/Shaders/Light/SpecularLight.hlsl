#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 viewDir : VIEWDIR;
};

PixelInput VS(VertexUVNormal input)
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    
    output.normal = mul(input.normal, (float3x3) world); // 
    //output.viewDir = normalize(output.pos.xyz - invView._41_42_43);   //
    output.viewDir = normalize(invView._31_32_33);  //forward    
    
    return output;
}

float4 PS(PixelInput input) : SV_TARGET
{    
    float4 albedo = diffuseMap.Sample(samp, input.uv);
    float3 normal = normalize(input.normal);
    float3 light = normalize(lightDirection);
    
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
    
    return ambient + diffuse + specular;
}