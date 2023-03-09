#include "framework.h"
#include "MineUI.h"

MineUI::MineUI()
{
	crossHair = new Quad(L"Textures/UI/cursor.png");
	crossHair->Pos() = Vector3(CENTER_X, CENTER_Y, 0);
	crossHair->UpdateWorld();

	quickSlot = new Quad(L"Textures/UI/QuickSlot.png");
	quickSlot->SetTag("QuickSlot");
	quickSlot->Load();
	quickSlot->UpdateWorld();

	vector<wstring> textures = {
		L"Textures/Block/UI_Dirt.png",
		L"Textures/Block/UI_Sand.png",
		L"Textures/Block/UI_Stone.png",
		L"Textures/Block/UI_Ice.png",
		L"Textures/Block/UI_Gold.png"
	};

	for (int i = 0; i < 5; i++) {
		Quad* icon = new Quad(textures[i]);
		icon->Scale() *= 0.1f;
		icon->SetParent(quickSlot);
		icon->SetTag("icon" + to_string(i));
		blockIcons[i] = icon;
	}

	iconFrame = new Quad(L"Textures/UI/IconFrame.png");
	iconFrame->SetTag("IconFrame");
	iconFrame->Load();
}

MineUI::~MineUI()
{
	delete crossHair;
	delete quickSlot;
	for (auto& blockIcon : blockIcons)
		delete blockIcon.second;

	delete iconFrame;
}

void MineUI::Update()
{
	for (int i = 0; i < 5; i++) {
		if (KEY_DOWN('1'+i) && iconData[i].second > 0)
				selectIconIndex = i;
	}

	if (selectIconIndex < 0)
		iconFrame->SetActive(false);
	else {
		iconFrame->SetActive(true);
		iconFrame->Pos() = blockIcons[iconData[selectIconIndex].first]->GlobalPos();
	}

	iconFrame->UpdateWorld();
}

void MineUI::PostRender()
{
	crossHair->Render();
	/*
	quickSlot->Render();
	for (int i = 0; i < 5; i++) {
		Vector3 pos = { 20.0f * (-4 + i), 0, 0 };
		if (iconData[i].second) {
			Quad* icon = blockIcons[iconData[i].first];
			icon->Pos() = pos;
			icon->UpdateWorld();
			icon->Render();

			string str = to_string(iconData[i].second);
			Vector3 temp = icon->GlobalPos();

			Font::Get()->RenderText(str, {temp.x, temp.y});
		}
	}
	iconFrame->Render();
	*/
}

void MineUI::GUIRender()
{
	if (ImGui::TreeNode("MineUI")) {
		quickSlot->RenderUI();

		for (auto& icon : blockIcons)
			icon.second->RenderUI();
		ImGui::TreePop();
	}
	iconFrame->RenderUI();
}

void MineUI::Mining()
{
	Cube* block = BlockManager::Get()->GetCollisionBlock();
	if (!block)
		return;


	iconFrame->SetActive(true);

	wstring file = block->GetMaterial()->GetDiffuseMap()->GetFile();
	int index = BlockManager::Get()->GetIndex(file);

	int inserted = -1;
	if (blocks[index].size() == 0) {
		for (int i = 0; i < 5; i++) {
			if (iconData[i].second == 0) {
				iconData[i].first = index;
				iconData[i].second = 1;
				inserted = i;
				break;
			}
		}
	}
	else {
		for (int i = 0; i < 5; i++) {
			if (iconData[i].first == index) {
				iconData[i].second++;
				inserted = i;
				break;
			}
		}
	}

	if (inserted >= 0 && selectIconIndex < 0)
		selectIconIndex = inserted;

	blocks[index].push_back(block);
}

void MineUI::Build()
{
	if (selectIconIndex < 0) 
		return;

	Cube* cube = blocks[iconData[selectIconIndex].first].back();

	if (BlockManager::Get()->BuildBlock(cube)) {
		blocks[iconData[selectIconIndex].first].pop_back();
		if (--iconData[selectIconIndex].second <= 0)
			selectIconIndex = -1;
	}
}

void MineUI::Save(wstring saveFile)
{
	BinaryWriter* writer = new BinaryWriter(saveFile);

	writer->UInt(iconData.size());
	for (auto& data : iconData) {
		writer->Int(data.first);
		writer->Int(data.second.first);
		writer->Int(data.second.second);
	}
	delete writer;
}

void MineUI::Load(wstring saveFile)
{
	BinaryReader* reader = new BinaryReader(saveFile);
	if (reader->IsFailed())
		return;

	//있던 정보는 싸그리 지우도록 한다
	iconData.clear();
	for (auto& p : blocks) {
		for(auto c : p.second)
			delete c;
		p.second.clear();
	}
	blocks.clear();

	UINT cnt = reader->UInt();	//개수
	for (UINT i = 0; i < cnt; i++) {
		int f = reader->Int();	//icon 자리
		int sf = reader->Int();	//블록 코드
		int ss = reader->Int();	//블록 개수

		for (int i = 0; i < ss; i++) {
			Cube* cube = new Cube();
			cube->GetMaterial()->SetDiffuseMap(BlockManager::Get()->GetTextureFile(sf));
			blocks[sf].push_back(cube);
		}

		iconData[f] = { sf, ss };
	}
	delete reader;

	selectIconIndex = -1;

}
