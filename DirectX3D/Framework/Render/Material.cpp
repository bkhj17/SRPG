#include "framework.h"
#include "Material.h"

Material::Material()
{
	diffuseMap = Texture::Add(DEFAULT_DIFFUSE_MAP, L"DM");
	specularMap = Texture::Add(DEFAULT_SPECULAR_MAP, L"SM");
	normalMap = Texture::Add(DEFAULT_NORMAL_MAP, L"NM");

	buffer = new MaterialBuffer;
}

Material::Material(wstring shaderFile)
{
	SetShader(shaderFile);
	diffuseMap = Texture::Add(DEFAULT_DIFFUSE_MAP);
	specularMap = Texture::Add(DEFAULT_SPECULAR_MAP);
	normalMap = Texture::Add(DEFAULT_NORMAL_MAP);

	buffer = new MaterialBuffer;
}

Material::~Material()
{
	delete buffer;
}

void Material::Set()
{
	diffuseMap->PSSet(0);
	specularMap->PSSet(1);
	normalMap->PSSet(2);

	buffer->SetPS(1);

	vertexShader->Set();
	pixelShader->Set();
}

void Material::RenderUI()
{
	string title = name + "_material";

	if (ImGui::TreeNode(title.c_str())) {
		ImGui::ColorEdit3("Diffuse", (float*)&buffer->Get().diffuse);
		ImGui::ColorEdit3("Specular", (float*)&buffer->Get().specular);
		ImGui::ColorEdit3("Ambient", (float*)&buffer->Get().ambient);

		ImGui::SliderFloat("Shininess", (float*)&buffer->Get().shininess, 1, 50);

		SelectMap("DM", DIFFUSE);
		ImGui::SameLine();
		UnSelectMap(DIFFUSE);

		SelectMap("SM", SPECULAR);
		ImGui::SameLine();
		UnSelectMap(DIFFUSE);

		SelectMap("NM", NORMAL);
		ImGui::SameLine();
		UnSelectMap(DIFFUSE);

		ImGui::TreePop();
	}
}

void Material::SetShader(wstring shaderFile)
{
	vertexShader = Shader::AddVS(shaderFile);
	pixelShader = Shader::AddPS(shaderFile);
}

void Material::SetDiffuseMap(wstring textureFile)
{
	diffuseMap = textureFile.length() > 0 ? Texture::Add(textureFile) : Texture::Add(DEFAULT_DIFFUSE_MAP);
}

void Material::SetSpecularMap(wstring textureFile)
{
	specularMap = textureFile.length() > 0 ? Texture::Add(textureFile) : Texture::Add(DEFAULT_SPECULAR_MAP);
}

void Material::SetNormalMap(wstring textureFile)
{
	if (textureFile.length() > 0) {
		buffer->Get().hasNormalMap = 1;
		normalMap = Texture::Add(textureFile);
	}
	else {
		buffer->Get().hasNormalMap = 0;
		Texture::Add(DEFAULT_NORMAL_MAP);
	}
}

void Material::SelectMap(string name, MapType type)
{

	ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.7f, 1), name.c_str());
	ImGui::SameLine();

	ImTextureID textureID = nullptr;
	switch (type)
	{
	case Material::DIFFUSE:
		textureID = diffuseMap->GetSRV();
		break;
	case Material::SPECULAR:
		textureID = specularMap->GetSRV();
		break;
	case Material::NORMAL:
		textureID = normalMap->GetSRV();
		break;
	default:
		break;
	}

	if (ImGui::ImageButton(textureID, ImVec2(50, 50))) {
		DIALOG->OpenDialog(this->name + name, name, ".png,.jpg,.tga", ".");
	}

	if (DIALOG->Display(this->name+name)) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);

			file = file.substr(strlen(path) + 1, file.length());

			switch (type)
			{
			case Material::DIFFUSE:
				SetDiffuseMap(ToWString(file));
				break;
			case Material::SPECULAR:
				SetSpecularMap(ToWString(file));
				break;
			case Material::NORMAL:
				SetNormalMap(ToWString(file));
				break;
			default:
				break;
			}
		}

		DIALOG->Close();
	}
}

void Material::UnSelectMap(MapType type)
{
	ImTextureID textureID = nullptr;
	switch (type)
	{
	case Material::DIFFUSE:
		textureID = Texture::Add(L"Textures/UI/Cancel.png", L"DMCancel")->GetSRV();
		break;
	case Material::SPECULAR:
		textureID = Texture::Add(L"Textures/UI/Cancel.png", L"SMCancel")->GetSRV();
		break;
	case Material::NORMAL:
		textureID = Texture::Add(L"Textures/UI/Cancel.png", L"NMCancel")->GetSRV();
		break;
	default:
		break;
	}

	if (ImGui::ImageButton(textureID, { 20,20 })) {
		switch (type)
		{
		case Material::DIFFUSE:
			SetDiffuseMap(L"");
			break;
		case Material::SPECULAR:
			SetSpecularMap(L"");
			break;
		case Material::NORMAL:
			SetNormalMap(L"");
			break;
		default:
			break;
		}
	}
}
