#include "framework.h"
#include "Cube230220.h"

Cube230220::Cube230220()
{
	float size = 1.0f;
	vertices.emplace_back(-size, -size, -size, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(-size, +size, -size, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(+size, -size, -size, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(+size, +size, -size, 0.0f, 0.0f, 1.0f);
	vertices.emplace_back(-size, -size, +size, 1.0f, 0.0f, 0.0f);
	vertices.emplace_back(-size, +size, +size, 1.0f, 1.0f, 0.0f);
	vertices.emplace_back(+size, -size, +size, 0.0f, 1.0f, 1.0f);
	vertices.emplace_back(+size, +size, +size, 0.0f, 0.0f, 1.0f);
	
	indices = {
		//Front
		0, 1, 2, 2, 1, 3,
		//Up
		1, 5, 3, 3, 5, 7,
		//Right
		2, 3, 6, 6, 3, 7,
		//Back
		6, 7, 4, 4, 7, 5,
		//Bottom
		4, 0, 6, 6, 0, 2,
		//Left
		4, 5, 0, 0, 5, 1
	};

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexColor), (UINT)vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), (UINT)indices.size());

	worldBuffer = new MatrixBuffer;
}

Cube230220::~Cube230220()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete worldBuffer;
}

void Cube230220::Update()
{
	S = XMMatrixScaling(localScale.x, localScale.y, localScale.z);
	R = XMMatrixRotationRollPitchYaw(localRotation.x, localRotation.y, localRotation.z);
	T = XMMatrixTranslation(localPosition.x, localPosition.y, localPosition.z);

	world = S * R * T;
}

void Cube230220::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();

	worldBuffer->Set(world);
	worldBuffer->SetVS(0);

	DC->DrawIndexed((UINT)indices.size(), 0, 0);
}

void Cube230220::GUIRender()
{
	string tag = "Cube";
	if (ImGui::TreeNode(tag.c_str()))
	{
		ImGui::Text(tag.c_str());

		string temp = tag + "_Pos";
		ImGui::DragFloat3(temp.c_str(), (float*)&localPosition, 1.0f);

		Float3 rot;
		rot.x = XMConvertToDegrees(localRotation.x);
		rot.y = XMConvertToDegrees(localRotation.y);
		rot.z = XMConvertToDegrees(localRotation.z);

		ImGui::DragFloat3((tag+"_Rot").c_str(), (float*)&rot, 1.0f, -180, 180);

		localRotation.x = XMConvertToRadians(rot.x);
		localRotation.y = XMConvertToRadians(rot.y);
		localRotation.z = XMConvertToRadians(rot.z);

		temp = tag + "_Scale";
		ImGui::DragFloat3(temp.c_str(), (float*)&localScale, 0.1f);

		ImGui::TreePop();
	}
}