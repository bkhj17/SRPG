#include "framework.h"
#include "ModelExportScene.h"

ModelExportScene::ModelExportScene()
{
	string name = "Robot";
	string file = "Models/FBX/" + name + ".fbx";
	
	//ModelExporter* exporter = new ModelExporter(name, file);
	//exporter->ExportMaterial();
	//exporter->ExportMesh();
	//delete exporter;

	string clipName = "Dance";
	file = "Models/Animations/" + name + "/" + clipName + ".fbx";
	ModelExporter* exporter = new ModelExporter(name, file);
	exporter->ExportClip(clipName);
	delete exporter;
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
