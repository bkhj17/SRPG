#include "Framework.h"

unordered_map<wstring, Shader*> Shader::shaders;

Shader::~Shader()
{
    blob->Release();
}

VertexShader* Shader::AddVS(wstring file)
{    
    if (shaders.count(file+L"VS") > 0)
        return (VertexShader*)shaders[file];

    wstring path = L"Shaders/" + file;
    wstring key = file + L"VS";
    shaders[key] = new VertexShader(path);

    return (VertexShader*)shaders[key];
}

PixelShader* Shader::AddPS(wstring file)
{
    if (shaders.count(file+L"PS") > 0)
        return (PixelShader*)shaders[file];

    wstring path = L"Shaders/" + file;
    wstring key = file + L"PS";
    shaders[key] = new PixelShader(path);

    return (PixelShader*)shaders[key];
}

void Shader::Delete()
{
    for (pair<wstring, Shader*> shader : shaders)
        delete shader.second;
}
