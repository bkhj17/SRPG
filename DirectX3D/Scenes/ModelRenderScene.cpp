#include "framework.h"
#include "ModelRenderScene.h"

ModelRenderScene::ModelRenderScene()
{
	model = new Model("Box01");
}

ModelRenderScene::~ModelRenderScene()
{
	delete model;
}

void ModelRenderScene::Update()
{
	model->UpdateWorld();
}

void ModelRenderScene::PreRender()
{
}

void ModelRenderScene::Render()
{
	model->Render();
}

void ModelRenderScene::PostRender()
{
}

void ModelRenderScene::GUIRender()
{
	ImGui::SliderInt("Mesh", &selectMeshNum, 0, model->GetMeshNum()-1);
	if (ImGui::Button("AddMaterial"))
		model->GetMesh(selectMeshNum)->SetMaterial(model->AddMaterial());
	ImGui::SameLine();
	if(ImGui::Button("SaveMaterial"))
		SaveMaterial();

	model->GUIRender();
}

void ModelRenderScene::SaveMaterial()
{
	string name = model->GetName();
	vector<Material*> materials = model->GetMatetials();

	string savePath = "Models/Materials/" + name + "/";
	string file = name + ".mats";

	CreateFolders(savePath);

	for (Material* material : materials) {
		string path = savePath + material->GetName() + ".mat";
		material->Save(path);
	}
}
