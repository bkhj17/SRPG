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
	void UpdateVertex(void* data, UINT count);
	//임의 추가
	void UpdateIndices(void* data, UINT count);

	vector<T>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }
private:
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

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
		DC->DrawIndexed(indices.size(), 0, 0);
	}
	else {
		DC->Draw(vertices.size(), 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	if(vertices.size() > 0)
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(T), vertices.size());
	if(indices.size() > 0)
		indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

template<typename T>
inline void Mesh<T>::UpdateVertex(void* data, UINT count)
{
	vertexBuffer->Update(data, count);
}

template<typename T>
inline void Mesh<T>::UpdateIndices(void* data, UINT count)
{
	indexBuffer->Update(data, count);
}
