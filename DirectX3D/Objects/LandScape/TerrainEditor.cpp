#include "framework.h"

TerrainEditor::TerrainEditor()
	: GameObject(L"LandScape/TerrainEditor.hlsl"), width(MAX_SIZE), height(MAX_SIZE)
{
	tag = "Terrain";

	mesh = new Mesh<VertexType>;
	MakeMesh();
	MakeNormal();
	MakeTangent();
	MakeComputeData();
	mesh->CreateMesh();

	brushBuffer = new BrushBuffer;
	rayBuffer = new RayBuffer();
}

TerrainEditor::~TerrainEditor()
{
	delete mesh;
	delete brushBuffer;

	delete rayBuffer;
	delete structuredBuffer;
}

void TerrainEditor::Update()
{
	brushBuffer->Get().pickingPos = ComputePicking();
}

void TerrainEditor::Render()
{
	brushBuffer->SetPS(10);
	SetRender();
	mesh->Draw();
}

void TerrainEditor::RenderUI()
{
	ImGui::Text("TerrainEdit Option");
	//ImGui::Text("x : %.1f, y : %.1f, z : %.1f", pickingPos.x, pickingPos.y, pickingPos.z);
	bool resize = false;
	resize |= ImGui::DragInt("Width", (int*)&width, 1.0f, MIN_SIZE, MAX_SIZE);
	resize |= ImGui::DragInt("Height", (int*)&height, 1.0f, MIN_SIZE, MAX_SIZE);
	if (resize)
		Resize();

	ImGui::DragFloat("Range", &brushBuffer->Get().range, 1.0f, 1.0f, 50.0f);
	ImGui::ColorEdit3("Color", (float*)&brushBuffer->Get().color);
}

Vector3 TerrainEditor::ComputePicking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
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
	for (OutputDesc output : outputs) {
		if (output.picked) {
			if (minDistance > output.distance) {
				minDistance = output.distance;
				minIndex = index;
			}
		}
		index++;
	}

	if (minIndex >= 0) {
		return ray.pos + ray.dir * minDistance;
	}

	return Vector3::Zero();
}

Vector3 TerrainEditor::Picking()
{
	Ray ray = CAM->ScreenPointToRay(mousePos);
	for (UINT z = 0; z < height - 1; z++)
	{
		for (UINT x = 0; x < width - 1; x++)
		{
			UINT index[4];
			index[0] = width * z + x;
			index[1] = width * z + x + 1;
			index[2] = width * (z + 1) + x;
			index[3] = width * (z + 1) + x + 1;

			vector<VertexType> vertices = mesh->GetVertices();

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].pos;

			float distance = 0.0f;
			if (Intersects(ray.pos, ray.dir, p[0], p[1], p[2], distance))	//Intersects : 속도 느림
			{
				//Vector3 result = ray.pos + ray.dir * distance;
				//result.z = height - result.z;
				//return result;
				return ray.pos + ray.dir * distance;
			}
			if (Intersects(ray.pos, ray.dir, p[3], p[1], p[2], distance))
			{
				//Vector3 result = ray.pos + ray.dir * distance;
				//result.z = height - result.z;
				//return result;
				return ray.pos + ray.dir * distance;
			}

			float check = distance;
		}
	}

	return Vector3();
}

void TerrainEditor::MakeMesh()
{
	vector<Float4> pixels(width * height, Float4(0, 0, 0, 0));
	if (heightMap) {
		width = (UINT)heightMap->GetSize().x;
		height = (UINT)heightMap->GetSize().y;

		heightMap->ReadPixels(pixels);
	}

	vector<VertexType>& vertices = mesh->GetVertices();
	vertices.clear();
	vertices.reserve((size_t)width * height);
	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			VertexType vertex;
			vertex.pos = { (float)x, 0.0f, (float)(height - z - 1) };
			vertex.uv.x = x / (float)(width - 1);
			vertex.uv.y = z / (float)(height - 1);

			UINT index = width * z + x;
			vertex.pos.y = pixels[index].x * MAX_HEIGHT;

			vertices.push_back(vertex);
		}
	}
	vector<pair<int, int>> dxz = {
		{0,0},
		{1,0},
		{0,1},
		{0,1},
		{1,0},
		{1,1}
	};

	vector<UINT>& indices = mesh->GetIndices();
	indices.clear();
	indices.reserve(((size_t)width - 1) * ((size_t)height - 1) * 6);
	for (int z = 0; z < height - 1; z++) {
		for (int x = 0; x < width - 1; x++) {
			for (int i = 0; i < 6; i++)
				indices.push_back(width * (z + dxz[i].second) + x + dxz[i].first);
		}
	}
}

void TerrainEditor::MakeNormal()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	const vector<UINT>& indices = mesh->GetIndices();
	for (UINT i = 0; i < indices.size() / 3; i++) {
		UINT index0 = indices[(size_t)i * 3 + 0];
		UINT index1 = indices[(size_t)i * 3 + 1];
		UINT index2 = indices[(size_t)i * 3 + 2];

		Vector3 pos0 = vertices[index0].pos;
		Vector3 pos1 = vertices[index1].pos;
		Vector3 pos2 = vertices[index2].pos;

		//순서 주의
		Vector3 e0 = pos1 - pos0;
		Vector3 e1 = pos2 - pos0;

		Vector3 normal = Cross(e0, e1).GetNormalized();

		vertices[index0].normal += normal;
		vertices[index1].normal += normal;
		vertices[index2].normal += normal;
		//정규화는 셰이더에서 하니 생략
	}
}

void TerrainEditor::MakeTangent()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	for (int i = 0; i * 3 < indices.size(); i++) {
		int index0 = indices[i * 3];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		Float2 uv0 = vertices[index0].uv;
		Float2 uv1 = vertices[index1].uv;
		Float2 uv2 = vertices[index2].uv;

		Vector3 e0 = (Vector3)vertices[index1].pos - vertices[index0].pos;
		Vector3 e1 = (Vector3)vertices[index2].pos - vertices[index0].pos;

		float u1 = uv1.x - uv0.x;
		float v1 = uv1.y - uv0.y;
		float u2 = uv2.x - uv0.x;
		float v2 = uv2.y - uv0.y;

		float d = 1.0f / (u1 * v2 - v1 * u2);
		Vector3 tangent = d * (e0 * v2 - e1 * v1);
		vertices[index0].tangent += tangent;
		vertices[index1].tangent += tangent;
		vertices[index2].tangent += tangent;
	}
}

void TerrainEditor::MakeComputeData()
{
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<UINT>& indices = mesh->GetIndices();

	computeShader = Shader::AddCS(L"Compute/ComputePicking.hlsl");
	triangleSize = indices.size() / 3;

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

	structuredBuffer = new StructuredBuffer(
		inputs.data(), sizeof(InputDesc), triangleSize,
		sizeof(OutputDesc), triangleSize);
}

void TerrainEditor::Resize()
{
	MakeMesh();
	MakeNormal();
	MakeTangent();
	MakeComputeData();

	mesh->UpdateVertex();
	mesh->UpdateIndex();
	structuredBuffer->UpdateInput(inputs.data());
}
