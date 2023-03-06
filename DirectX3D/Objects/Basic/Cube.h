#pragma once
class Cube : public GameObject
{
private:
	typedef VertexUV VertexType;

public:
	Cube(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	virtual ~Cube();

	virtual void Update();
	virtual void UpdateWorld();

	virtual void Render();

	class BoxCollider* GetCollider() { return collider; }
private:
	void MakeMesh();
private:
	Mesh<VertexUV>* mesh;

	Vector3 size;
	BoxCollider* collider;
};

