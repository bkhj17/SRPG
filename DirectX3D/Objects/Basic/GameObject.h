#pragma once

class GameObject : public Transform
{
public:
    GameObject(wstring shaderFile = L"Basic/Texture.hlsl");
    virtual ~GameObject();

    virtual void Render() {};
    virtual void SetRender();

    virtual void RenderUI();

    Material* GetMaterial() { return material; }
protected:
    Material* material;
    MatrixBuffer* worldBuffer;
};