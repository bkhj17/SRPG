#include "framework.h"
#include "WeaponManager.h"

WeaponManager::WeaponManager()
{
	LoadDatas();

	weaponPool.resize(50);
	for (auto& weapon : weaponPool) {
		weapon = new Weapon;
		weapon->SetActive(false);
	}
}

WeaponManager::~WeaponManager()
{
	datas.clear();

	for (auto& instance : instances)
		delete instance.second;
	instances.clear();

	for (auto& weapon : weaponPool)
		delete weapon;
	weaponPool.clear();
}

void WeaponManager::Update()
{
	for (auto& instance : instances)
		instance.second->Update();
}

void WeaponManager::Render()
{
	for (auto& instance : instances)
		instance.second->Render();
}

Weapon* WeaponManager::Pop(string key)
{
	WeaponData* data = GetData(key);
	if (data == nullptr)
		return nullptr;

	Weapon* weapon = nullptr;
	for (auto w : weaponPool) {
		if (w->Active())
			continue;
		weapon = w;
		weapon->SetActive(true);
		break;
	}
	if (weapon == nullptr)
		return nullptr;


	auto modelTransform = instances[data->modelName]->Add();
	modelTransform->Load(key);
	modelTransform->SetParent(weapon);

	weapon->SetData(data);
	weapon->SetModelTransform(key, modelTransform);

	return weapon;
}

void WeaponManager::LoadDatas()
{
	WeaponData data;
	data.name = "Sword";
	data.power = 1;
	data.hit = 100.0f;
	data.modelName = "Sword";
	instances[data.modelName] = new ModelInstancing(data.modelName);

	datas[data.name] = data;
}

WeaponData* WeaponManager::GetData(string key)
{
	if(datas.find(key) == datas.end())
		return nullptr;

	return &datas[key];
}
