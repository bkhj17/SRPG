#include "framework.h"
#include "SRPGUIManager.h"
#include "FloatingDamage.h"
#include "UIWindow.h"
#include "ActionSelectUI.h"
#include "MapSelectUI.h"
#include "AttackSelectUI.h"

SRPGUIManager::SRPGUIManager()
{
	floatingDamages.resize(50);
	for (auto& d : floatingDamages)
		d = new FloatingDamage(L"Textures/UI/Cancel.png");


	totalUI["ActionSelect"] = new ActionSelectUI(Vector3(200.0f, CENTER_Y));
	totalUI["MapSelectMove"] = new MapSelectUI("InputAction");
	totalUI["MapSelectAttack"] = new AttackSelectUI;

	Observer::Get()->AddEvent("BattleEnd", bind(&SRPGUIManager::CloseAll, this));
}

SRPGUIManager::~SRPGUIManager()
{
	for (auto d : floatingDamages)
		delete d;

	for (auto& ui : totalUI)
		delete ui.second;
}

void SRPGUIManager::Update()
{
	for (auto d : floatingDamages)
		d->Update();

	while (!openned.empty() && !openned.back()->Active())
		openned.pop_back();

	//앞에서 빼기 때문에 축약 불가
	if (!openned.empty())
		openned.back()->Update();
}

void SRPGUIManager::Render()
{
	for (auto d : floatingDamages)
		d->Render();

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