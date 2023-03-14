#include "framework.h"
#include "Terrain230313.h"

Terrain230313::Terrain230313()
{
	MakeComputeData();

	computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");
	structuredBuffer = new StructuredBuffer(
		inputs.data(), sizeof(InputDesc), triangleSize,
		sizeof(OutputDesc), triangleSize);
	rayBuffer = new RayBuffer();
}

Terrain230313::~Terrain230313()
{
	delete rayBuffer;
	delete structuredBuffer;
}

bool Terrain230313::ComputePicking(Vector3& pos)
{
	Ray ray = CAM->ScreenPointToRay(Vector3(CENTER_X, CENTER_Y));
	rayBuffer->Get().pos = ray.pos;
	rayBuffer->Get().dir = ray.dir;
	rayBuffer->Get().triangleSize = triangleSize;

	rayBuffer->SetCS(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->GetSRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->GetUAV(), nullptr);

	computeShader->Set();

	UINT x = (UINT)ceil((float)triangleSize / 64.0f);
	DC->Dispatch(x, 1, 1);	//연산 실행
	structuredBuffer->Copy(outputs.data(), sizeof(OutputDesc) * triangleSize);

	float minDistance = FLT_MAX;
	int minIndex = -1;
	UINT index = 0;
	for (const OutputDesc& output : outputs) {
		if (output.picked) {
			if (minDistance > output.distance) {
				minDistance = output.distance;
				minIndex = index;
			}
		}
		index++;
	}

	if (minIndex >= 0) {
		pos = ray.pos + ray.dir * minDistance;

		return true;
	}

	return false;
}

void Terrain230313::MakeComputeData()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	triangleSize = (UINT)indices.size() / 3;

	inputs.resize(triangleSize);
	outputs.resize(triangleSize);

	for (UINT i = 0; i < triangleSize; i++) {
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		inputs[i].v0 = vertices[index0].pos;
		inputs[i].v1 = vertices[index1].pos;
		inputs[i].v2 = vertices[index2].pos;
	}
}
