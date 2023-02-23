//Pixel ConstBuffer
cbuffer LightBuffer : register(b0)
{
    float3 lightDirection;
    float shininess;
}

//Texture & Sampler
SamplerState samp : register(s0);
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);