#include "framework.h"
#include "SRPGUIManager.h"
#include "FloatingDamage.h"

SRPGUIManager::SRPGUIManager()
{
	floatingDamages.resize(50);
	for (auto& d : floatingDamages)
		d = new FloatingDamage(L"Textures/UI/Cancel.png");

}

SRPGUIManager::~SRPGUIManager()
{
	for (auto d : floatingDamages)
		delete d;
}

void SRPGUIManager::Update()
{
	for (auto d : floatingDamages)
		d->Update();
}

void SRPGUIManager::Render()
{
	for (auto d : floatingDamages)
		d->Render();

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
