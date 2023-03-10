#include "framework.h"
#include "Sphere230223.h"

Sphere230223::Sphere230223(float radius, int numSlices, int numStacks)
    : GameObject(L"Light/SpecularLight230223.hlsl")
{
    tag = "Sphere";
    mesh = new Mesh<VertexType>;

    MakeMesh(radius, numSlices, numStacks);

    mesh->CreateMesh();

    intensityBuffer = new FloatValueBuffer;
    intensityBuffer->Get()[0] = 1.0f;

}

Sphere230223::~Sphere230223()
{
	delete mesh;

    delete intensityBuffer;
}

void Sphere230223::Update()
{
	if (!isActive)
		return;

	UpdateWorld();
}

void Sphere230223::Render()
{
	if (!isActive)
		return;

    if (ambientMap)
        ambientMap->PSSet(2);
    intensityBuffer->SetPS(1);
	SetRender();
	mesh->Draw();
}

void Sphere230223::GUIRender()
{
	if (ImGui::TreeNode(tag.c_str())) {
		Transform::GUIRender();
		material->GUIRender();

		string temp = tag + "_Shininess";
		ImGui::DragFloat(temp.c_str(), (float*)&intensityBuffer->Get()[0], 0.1f, 0.0f);

		temp = tag + "_Ambient";
        ImTextureID textureID = ambientMap ? ambientMap->GetSRV() : nullptr;
        if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
            DIALOG->OpenDialog(tag + "AmbientMap", "AmbientMap", ".png,.jpg", ".");
        }

		ImGui::TreePop();
	}

    if (DIALOG->Display(tag + "AmbientMap")) {
        if (DIALOG->IsOk()) {
            string file = DIALOG->GetFilePathName();
            char path[128];
            GetCurrentDirectoryA(128, path);

            file = file.substr(strlen(path) + 1, file.length());

            wstring textureFile = ToWString(file);
            ambientMap = textureFile.length() > 0 ? Texture::Add(textureFile) : nullptr;
        }

        DIALOG->Close();
    }
}

void Sphere230223::MakeMesh(float radius, int numSlices, int numStacks)
{
    float phiStep = XM_PI / numStacks;
    float thetaStep = XM_2PI / numSlices;

    vector<VertexType>& vertices = mesh->GetVertices();
    vertices.reserve((numStacks+1) * (numSlices+1));
    for (int y = 0; y <= numStacks; ++y) {
        float phi = phiStep * y;
        float cosPhi = cos(phi);
        float sinPhi = sin(phi);

        for (int x = 0; x <= numSlices; ++x) {
            float theta = thetaStep * x;
            float cosTheta = cos(theta);
            float sinTheta = sin(theta);

            VertexType vertex;
            vertex.normal.x = sinPhi * cosTheta;
            vertex.normal.y = cosPhi;
            vertex.normal.z = sinPhi * sinTheta;
            vertex.pos = Vector3(vertex.normal) * radius;
            vertex.uv.x = (float)x / numSlices;
            vertex.uv.y = (float)y / numStacks;
            vertices.push_back(vertex);
        }
    }

    vector<UINT>& indices = mesh->GetIndices();
    indices.reserve(numStacks * numSlices * 6);
    for (int i = 0; i < numStacks; ++i) {
        for (int j = 0; j < numSlices; ++j) {
            int k1 = i * (numSlices + 1) + j;
            int k2 = k1 + 1;
            int k3 = (i + 1) * (numSlices + 1) + j;
            int k4 = k3 + 1;

            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k3);

            indices.push_back(k2);
            indices.push_back(k4);
            indices.push_back(k3);
        }
    }
}