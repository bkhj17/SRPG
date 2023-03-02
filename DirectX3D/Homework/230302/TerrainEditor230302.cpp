#include "framework.h"
#include "TerrainEditor230302.h"

TerrainEditor230302::TerrainEditor230302()
{
	material->SetNormalMap(L"Textures/LandScape/Dirt_NM.png");
	material->SetSpecularMap(L"Textures/Landscape/Dirt_SM.png");

	mapPack[0].diffuseMap = Texture::Add(L"Textures/LandScape/Floor.png");
	mapPack[0].normalMap = Texture::Add(L"Textures/LandScape/Floor_normal.png");
	mapPack[0].specularMap = Texture::Add(L"Textures/LandScape/Floor_SM.png");


	mapPack[1].diffuseMap = Texture::Add(L"Textures/LandScape/Stones.png");
	mapPack[1].normalMap = Texture::Add(L"Textures/LandScape/Stones_normal.png");
	mapPack[1].specularMap = Texture::Add(L"Textures/LandScape/Stones_SM.png");
}

void TerrainEditor230302::Render()
{
	brushBuffer->SetPS(10);

	mapPack[0].Set(1);
	mapPack[1].Set(2);

	SetRender();
	mesh->Draw();
}

void TerrainEditor230302::RenderUI()
{
	__super::RenderUI();

	if (ImGui::Button("Save_Terrain")) {
		DIALOG->OpenDialog("Save_Terrain", "Save_Terrain", "", ".");
	}

	if (ImGui::Button("Load_Terrain")) {
		DIALOG->OpenDialog("Load_Terrain", "Load_Terrain", "", ".");
	}

	if (DIALOG->Display("Save_Terrain")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);
			file = file.substr(strlen(path) + 1, file.length());
			Save(file);
		}
		DIALOG->Close();
	}


	if (DIALOG->Display("Load_Terrain")) {
		if (DIALOG->IsOk()) {
			string file = DIALOG->GetFilePathName();
			char path[128];
			GetCurrentDirectoryA(128, path);
			file = file.substr(strlen(path) + 1, file.length());
			Load(file);
		}
		DIALOG->Close();
	}

}

void TerrainEditor230302::Save(string file)
{
	BinaryWriter* writer = new BinaryWriter(file);

	writer->WString(material->GetDiffuseMap()->GetFile());
	writer->WString(material->GetSpecularMap()->GetFile());
	writer->WString(material->GetNormalMap()->GetFile());

	writer->WString(mapPack[0].diffuseMap->GetFile());
	writer->WString(mapPack[0].specularMap->GetFile());
	writer->WString(mapPack[0].normalMap->GetFile());

	writer->WString(mapPack[1].diffuseMap->GetFile());
	writer->WString(mapPack[1].specularMap->GetFile());
	writer->WString(mapPack[1].normalMap->GetFile());


	wstring heightMapFile = ToWString(file) + L"_Height.png";
	writer->WString(heightMapFile);

	wstring alphaMapFile = ToWString(file) + L"_Alpha.png";
	writer->WString(alphaMapFile);

	delete writer;

	{
		UINT size = width * height * 4;	//4 : rgba

		vector<VertexType>& vertices = mesh->GetVertices();

		uint8_t* pixels = new uint8_t[size];
		for (UINT i = 0; i < size / 4; i++) {
			float y = vertices[i].pos.y;
			uint8_t height = (y - MIN_HEIGHT) / (MAX_HEIGHT - MIN_HEIGHT) * 255;
			pixels[i * 4 + 0] = height;
			pixels[i * 4 + 1] = height;
			pixels[i * 4 + 2] = height;
			pixels[i * 4 + 3] = 255;
		}

		Image image;
		image.width = width;
		image.height = height;
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
		image.rowPitch = width * 4;
		image.slicePitch = size;
		image.pixels = pixels;
		SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
			GetWICCodec(WIC_CODEC_PNG), heightMapFile.c_str());

		delete[] pixels;
	}

	{
		UINT size = width * height * 4;	//4 : rgba
		uint8_t* pixels = new uint8_t[size];

		vector<VertexType>& vertices = mesh->GetVertices();

		for (UINT i = 0; i < size / 4; i++) {
			pixels[i * 4 + 0] = vertices[i].alpha[0] * 255;
			pixels[i * 4 + 1] = vertices[i].alpha[1] * 255;
			pixels[i * 4 + 2] = vertices[i].alpha[2] * 255;
			pixels[i * 4 + 3] = 255;
		}

		Image image;
		image.width = width;
		image.height = height;
		image.format = DXGI_FORMAT_R8G8B8A8_UNORM;
		image.rowPitch = width * 4;
		image.slicePitch = size;
		image.pixels = pixels;
		SaveToWICFile(image, WIC_FLAGS_FORCE_RGB,
			GetWICCodec(WIC_CODEC_PNG), alphaMapFile.c_str());

		delete[] pixels;
	}
}

void TerrainEditor230302::Load(string file)
{
	BinaryReader* reader = new BinaryReader(file);
	material->SetDiffuseMap(reader->WString());
	material->SetSpecularMap(reader->WString());
	material->SetNormalMap(reader->WString());

	mapPack[0].diffuseMap = Texture::Add(reader->WString());
	mapPack[0].specularMap = Texture::Add(reader->WString());
	mapPack[0].normalMap = Texture::Add(reader->WString());

	mapPack[1].diffuseMap = Texture::Add(reader->WString());
	mapPack[1].specularMap = Texture::Add(reader->WString());
	mapPack[1].normalMap = Texture::Add(reader->WString());

	wstring heightMapFile = reader->WString();
	wstring alphaMapFile = reader->WString();

	delete reader;

	heightMap = Texture::Add(heightMapFile);
	Resize();



	Texture* alphaMap = Texture::Add(alphaMapFile);
	vector<VertexType>& vertices = mesh->GetVertices();
	vector<Float4> pixels;
	alphaMap->ReadPixels(pixels);
	for (UINT i = 0; i < vertices.size(); i++) {
		vertices[i].alpha[0] = pixels[i].z;
		vertices[i].alpha[1] = pixels[i].y;
		vertices[i].alpha[2] = pixels[i].x;
		vertices[i].alpha[3] = pixels[i].w;
	}

	mesh->UpdateVertex();

}
