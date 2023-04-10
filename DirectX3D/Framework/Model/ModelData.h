#pragma once

typedef VertexUVNormalTangentBlend ModelVertex;

struct MeshData
{
	string name;
	UINT materialIndex = 0;

	vector<ModelVertex> vertices;
	vector<UINT> indices;
};

struct NodeData {
	int index = 0;
	string name;
	int parent = -1;
	Matrix transform = {};
};

struct BoneData {
	int index = 0;
	string name;
	Matrix offset = {};
};

struct VertexWeights
{
	UINT indices[4] = {};	//Bone Index
	float weights[4] = {};	//Bone 가중치 : 보간할 때 필요

	void Add(const UINT& index, const float& weight) {
		for (int i = 0; i < 4; i++) {
			if (weights[i] == 0.0f) {
				indices[i] = index;
				weights[i] = weight;
				return;
			}
		}
	}

	void Normalize() {
		float sum = 0.0f;
		for (int i = 0; i < 4; i++)
			sum += weights[i];

		for (UINT i = 0; i < 4; i++)
			weights[i] /= sum;
	}
};

struct KeyVector {
	float time = 0.0f;
	Float3 value = {};
};

struct KeyQuat {
	float time = 0.0f;
	Float4 value = {};
};

struct KeyData {
	vector<KeyVector> scales;
	vector<KeyQuat> rotations;
	vector<KeyVector> positions;
};

struct KeyTransform
{
	Float3 scale = {};
	Float4 rot = {};
	Float3 pos = {};
};

struct KeyFrame {
	string boneName;
	vector<KeyTransform> transforms;
};

struct ClipNode {
	aiString name;
	vector<KeyTransform> transforms;
};

struct Clip {
	string name;
	UINT frameCount = 0;
	float tickPerSecond = 0.0f;

	vector<KeyFrame*> keyFrame;
};

struct ClipTransform {
	Matrix transform[MAX_FRAME][MAX_BONE];
};