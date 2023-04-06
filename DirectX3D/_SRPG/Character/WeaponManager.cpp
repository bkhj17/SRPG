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
	Weapon::WeaponData* data = GetData(key);
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
	Weapon::WeaponData data;
	data.name = "Sword";
	data.type = Weapon::SWORD;
	data.power = 2;
	data.hit = 100.0f;
	data.range = { 1, 1 };
	data.modelName = "Sword";
	instances[data.modelName] = new ModelInstancing(data.modelName);
	datas[data.name] = data;

	data.name = "Axe";
	data.type = Weapon::AXE;
	data.power = 4;
	data.hit = 85.0f;
	data.range = { 1, 1 };
	data.modelName = "Axe";
	instances[data.modelName] = new ModelInstancing(data.modelName);
	datas[data.name] = data;

	data.name = "Bow";
	data.type = Weapon::BOW;
	data.power = 2;
	data.hit = 90.0f;
	data.range = { 2, 2 };
	data.modelName = "Bow";
	instances[data.modelName] = new ModelInstancing(data.modelName);
	datas[data.name] = data;
}

Weapon::WeaponData* WeaponManager::GetData(string key)
{
	if(datas.find(key) == datas.end())
		return nullptr;

	return &datas[key];
}
