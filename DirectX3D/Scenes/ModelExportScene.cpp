#include "framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	string name = "Box01";
	string file = "Models/FBX/" + name + ".fbx";
	ModelExporter* exporter = nullptr;
	
	exporter = new ModelExporter(name, file);
	exporter->ExportMaterial();
	exporter->ExportMesh();
	delete exporter;
	
	vector<string> clipNames = {
//		"Hit",
//		"SpearIdle",
//		"SpearAttack",
//		"Run",
//		"AxeIdle",
//		"AxeAttack",
//		"SwordIdle",
//		"SwordAttack",
//		"Death",
//		"BowAttack"
	};
	
	for (const auto& clipName : clipNames) {
		file = "Models/Animations/" + name + "/" + clipName + ".fbx";
		exporter = new ModelExporter(name, file);
		exporter->ExportClip(clipName);
		delete exporter;
	}
}

void ModelExportScene::Update()
{
}

void ModelExportScene::PreRender()
{
}

void ModelExportScene::Render()
{
}

void ModelExportScene::PostRender()
{
}

void ModelExportScene::GUIRender()
{
}
