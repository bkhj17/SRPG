#pragma once

class Cube230223 : public GameObject
{
private:
	typedef VertexUVNormal VertexType;

public:
	Cube230223(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	virtual ~Cube230223();

	virtual void Update();
	virtual void Render();

	virtual void RenderUI();
private:
	void MakeMesh(Vector3 size);

private:
	Mesh<VertexUVNormal>* mesh;

	Texture* ambientMap = nullptr;
	FloatValueBuffer* intensityBuffer;
};

