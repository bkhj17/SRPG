#include "framework.h"
#include "Material.h"

Material::Material()
{
}

Material::Material(wstring shaderFile)
{
	SetShader(shaderFile);
	SetDiffuseMap(DEFAULT_DIFFUSE_MAP);
}

Material::~Material()
{
}

void Material::Set()
{
	if (diffuseMap)
		diffuseMap->PSSet(0);
	else
		Texture::Add(DEFAULT_DIFFUSE_MAP)->PSSet(0);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::RenderUI()
{
	string title = name + "_material";
	if (ImGui::TreeNode(title.c_str())) {
		ImTextureID textureID = diffuseMap->GetSRV();
		
		if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
			DIALOG->OpenDialog("Map", "Map", ".png,.jpg", ".");
		}
		ImGui::TreePop();
	}

	if (DIALOG->Display("Map")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);

			file = file.substr(strlen(path) + 1, file.length());
			SetDiffuseMap(ToWString(file));
		}

		DIALOG->Close();
	}
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	diffuseMap = textureFile.length() > 0 ? Texture::Add(textureFile) : nullptr;
}
