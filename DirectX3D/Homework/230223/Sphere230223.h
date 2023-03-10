#pragma once
class Sphere230223 : public GameObject
{
private:
	typedef VertexUVNormal VertexType;

public:
	Sphere230223(float radius = 1.0f, int numSlices = 36, int numStacks = 36);
	virtual ~Sphere230223();

	virtual void Update();
	virtual void Render();
	virtual void GUIRender();
private:
	void MakeMesh(float radius, int numSlices, int numStacks);

private:
	Mesh<VertexType>* mesh;

	Texture* ambientMap = nullptr;
	FloatValueBuffer* intensityBuffer;
};

