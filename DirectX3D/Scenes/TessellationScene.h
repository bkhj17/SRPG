#pragma once

class TessellationScene : public Scene
{
private:
	const wstring TRIANGLE_SHADER = L"TS/Triangle.hlsl";
	const wstring RECTANGLE_SHADER = L"TS/Rectangle.hlsl";

public:
	TessellationScene();
	~TessellationScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void GUIRender() override;

private:
	Material* material;
	HullShader* hullShader;
	DomainShader* domainShader;

	Mesh<Vertex>* mesh;

	FloatValueBuffer* edgeBuffer;
	FloatValueBuffer* insideBuffer;
};

