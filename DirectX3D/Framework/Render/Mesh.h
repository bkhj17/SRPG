#pragma once
template<typename T>
class Mesh
{
public:
	Mesh() = default;
	virtual ~Mesh();

	void Draw(D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void CreateMesh();
	void UpdateVertex();
	void UpdateIndex();

	vector<T>& GetVertices() { return vertices; }
	vector<UINT>& GetIndices() { return indices; }

	VertexBuffer* GetVertexBuffer() { return vertexBuffer; }
	IndexBuffer* GetIndexBuffer() { return indexBuffer; }

	void SetDebugName(string name = "");
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
inline void Mesh<T>::DrawInstanced(UINT instanceCount, D3D11_PRIMITIVE_TOPOLOGY type)
{
	if (!vertexBuffer)
		return;

	vertexBuffer->Set(type);

	if (indexBuffer) {
		indexBuffer->Set();
		DC->DrawIndexedInstanced((UINT)indices.size(), instanceCount, 0, 0, 0);
	}
	else {
		DC->DrawInstanced((UINT)vertices.size(), instanceCount, 0, 0);
	}
}

template<typename T>
inline void Mesh<T>::CreateMesh()
{
	if (vertices.size() > 0) {
		if (vertexBuffer)
			delete vertexBuffer;
		vertexBuffer = new VertexBuffer(vertices.data(), sizeof(T), (UINT)vertices.size());
	}
	if (indices.size() > 0) {
		if (indexBuffer)
			delete indexBuffer;
		indexBuffer = new IndexBuffer(indices.data(), (UINT)indices.size());
	}

	SetDebugName();
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

template<typename T>
inline void Mesh<T>::SetDebugName(string name)
{
	if(vertexBuffer)
		vertexBuffer->SetDebugName(name + "->Mesh");
	if(indexBuffer)
		indexBuffer->SetDebugName(name + "->Mesh");
}
