#include "framework.h"
#include "Material.h"

Material::Material()
{
}

Material::Material(wstring shaderFile)
{
	SetShader(shaderFile);
	SetDiffuseMap(DEFAULT_DIFFUSE_MAP);
	SetSpecularMap(DEFAULT_SPECULAR_MAP);
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

	if (specularMap)
		specularMap->PSSet(1);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::RenderUI()
{
	string title = name + "_material";

	if (ImGui::TreeNode(title.c_str())) {
		ImTextureID textureID = diffuseMap->GetSRV();
		
		if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
			selected = "DiffuseMap";
			DIALOG->OpenDialog("Map", selected, ".png,.jpg", ".");
		}

		textureID = specularMap ? specularMap->GetSRV() : nullptr;
		if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
			selected = "SpecularMap";
			DIALOG->OpenDialog("Map", selected, ".png,.jpg", ".");
		}
		ImGui::TreePop();
	}

	if (DIALOG->Display("Map")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);

			file = file.substr(strlen(path) + 1, file.length());

			if(selected.compare("DiffuseMap") == 0)
				SetDiffuseMap(ToWString(file));
			else if(selected.compare("SpecularMap") == 0)
				SetSpecularMap(ToWString(file));
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

void Material::SetSpecularMap(wstring textureFile)
{
	specularMap = textureFile.length() > 0 ? Texture::Add(textureFile) : nullptr;
}
