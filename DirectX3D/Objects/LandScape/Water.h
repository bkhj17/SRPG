#pragma once

class Water : public GameObject
{
public:
	Water(wstring normalFile, float width = 100.0f, float height = 100.0f);
	~Water();

	void Update();
	void Render();
	void GUIRender();

	void SetReflection();	//¹Ý»ç
	void SetRefraction();	//±¼Àý
private:
	void MakeMesh();

private:
	Vector2 size;

	Mesh<VertexUV>* mesh;

	Reflection* reflection;
	Refraction* refraction;

	BlendState* blendState[2];
};

