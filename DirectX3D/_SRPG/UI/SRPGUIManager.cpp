#include "framework.h"
#include "SRPGUIManager.h"
#include "FloatingDamage.h"
#include "UIWindow.h"

SRPGUIManager::SRPGUIManager()
{
	floatingDamages.resize(50);
	for (auto& d : floatingDamages)
		d = new FloatingDamage(L"Textures/UI/Cancel.png");

	totalUI["Test"] = new UIWindow(Vector2(300.0f, 400.0f), Vector3(CENTER_X, CENTER_Y));
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

	if (!openned.empty())
		openned.back()->Update();
}

void SRPGUIManager::Render()
{
	for (auto d : floatingDamages)
		d->Render();

	for (auto window : openned)
		window->Render();

	if (!openned.empty())
		openned.back()->RenderCursor();
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

	openned.push_back(totalUI[key]);
	openned.back()->Init();

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
