#pragma once

class Cube230303 : public GameObject
{
private:
	typedef VertexUVNormalTangent VertexType;

public:
	Cube230303(Vector3 size = Vector3(1.0f, 1.0f, 1.0f));
	~Cube230303();

	void UpdateWorld() override;
	void Render() override;

	BoxCollider* GetCollider() { return collider; }
private:
	void MakeMesh(Vector3 size);
	void MakeTangent();
private:
	Mesh<VertexType>* mesh;


	BoxCollider* collider;
};

