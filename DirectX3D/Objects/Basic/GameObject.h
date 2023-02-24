#pragma once

class GameObject : public Transform
{
public:
    GameObject(wstring shaderFile = L"Light/NormalMapping.hlsl");
    virtual ~GameObject();

    virtual void Render() {};
    virtual void SetRender();

    virtual void RenderUI() override;

    Material* GetMaterial() { return material; }
protected:
    Material* material;
    MatrixBuffer* worldBuffer;
};