#pragma once
template<typename T>
class Mesh
{
private:
	typedef VertexUV VertexType;
public:
	Mesh() = default;
	~Mesh();

	void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void CreateMesh();
	void UpdateVertex();
	void UpdateIndex();

	vector<T>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }
private:
	VertexBuffer* vertexBuffer = nullptr;
	IndexBuffer* indexBuffer = nullptr;

	vector<T> vertices;
	vector<UINT> indices;
};

template<typename T>
inline Mesh<T>::~Mesh()
{
	delete vertexBuffer;
	delete indexBuffer;
}

template<typename T>
inline void Mesh<T>::Draw(D3D11_PRIMITIVE_TOPOLOGY type)
{
	if (!vertexBuffer)
		return;

	vertexBuffer->Set(type);
	
	if (indexBuffer) {
		indexBuffer->Set();
		DC->DrawIndexed((UINT)indices.size(), 0, 0);
	}
	else {
		DC->Draw((UINT)vertices.size(), 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	if(vertices.size() > 0)
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(T), (UINT)vertices.size());
	if(indices.size() > 0)
		indexBuffer = new IndexBuffer(indices.data(), (UINT)indices.size());
}

template<typename T>
inline void Mesh<T>::UpdateVertex()
{
	vertexBuffer->Update(vertices.data(), (UINT)vertices.size());
}

template<typename T>
inline void Mesh<T>::UpdateIndex()
{
	indexBuffer->Update(indices.data(), (UINT)indices.size());
}
