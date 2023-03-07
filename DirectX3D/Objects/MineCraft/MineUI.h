#pragma once
class MineUI
{
public:
	MineUI();
	~MineUI();

	void Update();
	void PostRender();
	void GUIRender();

	void Mining();
	void Build();

	void Save(wstring saveFile);
	void Load(wstring saveFile);
private:
	Quad* crossHair;
	//인벤토리
	Quad* quickSlot;
	Quad* iconFrame;

	map<int, Quad*> blockIcons;
	map<int, pair<int, int>> iconData;
	int selectIconIndex = -1;

	map<int, vector<Cube*>> blocks;
};
