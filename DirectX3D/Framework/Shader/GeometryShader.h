#pragma once
class GeometryShader : public Shader
{
private:
	friend class Shader;
	GeometryShader(wstring file);
	virtual ~GeometryShader();

public:
	virtual void Set() override;

private:
	ID3D11GeometryShader* shader = nullptr;
};

