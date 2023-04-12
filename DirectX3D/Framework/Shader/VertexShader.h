#pragma once

class VertexShader : public Shader
{
private:
    friend class Shader;

    VertexShader(wstring file);
    virtual ~VertexShader();

public:
    virtual void Set() override;

private:
    void CreateInputLayout();

private:
    ID3D11VertexShader* shader = nullptr;
    ID3D11InputLayout* inputLayout = nullptr;

    ID3D11ShaderReflection* reflection;
};