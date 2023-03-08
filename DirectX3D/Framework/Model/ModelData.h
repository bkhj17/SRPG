#pragma once

typedef VertexUVNormalTangent ModelVertex;

struct MeshData
{
	string name;
	UINT materialIndex;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};