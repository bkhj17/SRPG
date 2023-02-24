//Pixel ConstBuffer
cbuffer LightBuffer : register(b0)
{
    float4 lightColor;
    float3 lightDirection;
    float padding;
    float4 ambientColor;
}

cbuffer MaterialBuffer : register(b1)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float shininess;
    int hasNormalMap;
}

//Texture & Sampler
SamplerState samp : register(s0);
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);