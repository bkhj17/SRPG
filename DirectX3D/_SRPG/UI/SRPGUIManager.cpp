#include "framework.h"

SRPGUIManager::SRPGUIManager()
{
	floatingDamages.resize(10);
	for (auto& d : floatingDamages)
		d = new FloatingDamage(L"Textures/UI/Cancel.png");

	totalUI["MainSelect"] = new MainSelectUI(Vector3(200.0f, CENTER_Y));
	totalUI["ActionSelect"] = new ActionSelectUI(Vector3(200.0f, CENTER_Y));
	totalUI["MapSelectMove"] = new MapSelectUI("InputAction");
	totalUI["MapSelectAttack"] = new AttackSelectUI;
	totalUI["TurnChangeUI"] = new TurnChangeUI;

	totalUI["Info1"] = new InfoUI(Vector3(200.0f, 150.0f));
	totalUI["Info2"] = new InfoUI(Vector3(WIN_WIDTH - 200.0f, 150.0f));
	totalUI["Turn"] = new TurnUI(Vector3(CENTER_X, WIN_HEIGHT - 30));

	Observer::Get()->AddEvent("BattleEnd", bind(&SRPGUIManager::CloseAll, this));
	Observer::Get()->AddParamEvent("SetInfo", bind(&SRPGUIManager::SetInfo, this, placeholders::_1));
}

SRPGUIManager::~SRPGUIManager()
{
	openned.clear();

	for (auto d : floatingDamages)
		delete d;

	for (auto& ui : totalUI)
		delete ui.second;
}

void SRPGUIManager::Update()
{
	for (auto d : floatingDamages)
		d->Update();

	//꺼진 UI들 빼기
	while (!openned.empty() && !openned.back()->Active())
		openned.pop_back();

	//앞에서 빼기 때문에 축약 불가
	if (!openned.empty())
		openned.back()->Update();

	totalUI["Info1"]->UpdateWorld();
	totalUI["Info2"]->UpdateWorld();
	totalUI["Turn"]->UpdateWorld();
}

void SRPGUIManager::Render()
{
	for (auto d : floatingDamages)
		d->Render();

	totalUI["Info1"]->Render();
	totalUI["Info2"]->Render();
	totalUI["Turn"]->Render();

	if (!openned.empty())
		openned.back()->Render();

}

void SRPGUIManager::SpawnDamage(Vector3 pos, int damage)
{
	for (auto d : floatingDamages) {
		if (d->Active())
			continue;

		d->Spawn(pos);
		break;
	}
}

void SRPGUIManager::OpenMenu()
{
	OpenUI("Menu");
}

void SRPGUIManager::CloseAll()
{
	openned.clear();
}

UIWindow* SRPGUIManager::OpenUI(string key)
{
	if (totalUI.find(key) == totalUI.end())
		return nullptr;

	totalUI[key]->Init();
	openned.push_back(totalUI[key]);

	return openned.back();
}

UIWindow* SRPGUIManager::OpenUI(string key, Vector3 pos)
{
	UIWindow* opened = OpenUI(key);
	if (opened) {
		opened->Pos() = pos;
		opened->UpdateWorld();
	}
	return opened;
}

void SRPGUIManager::CloseUI()
{
	if (openned.empty())
		return;
	openned.pop_back();
}

void SRPGUIManager::CloseUI(string key)
{
	if (totalUI.find(key) == totalUI.end())
		return;

	totalUI[key]->Close();
}

bool SRPGUIManager::IsActing()
{
	return !openned.empty();
}

bool SRPGUIManager::IsMapControl()
{
	return !openned.empty() && openned.back()->IsMapControl();
}

void SRPGUIManager::SetInfo(void* characterPtr)
{
	auto character = (Character*)characterPtr;
	if (character != nullptr && !character->Active())
		character = nullptr;

	auto info1 = (InfoUI*)totalUI["Info1"];
	auto info2 = (InfoUI*)totalUI["Info2"];

	auto holded = CharacterManager::Get()->HoldedCharacter();
	if (holded && holded != character) {
		info1->SetCharacter(holded);
		info2->SetCharacter(character);
	}
	else {
		info1->SetCharacter(character);
		info2->SetCharacter(nullptr);
	}
}
