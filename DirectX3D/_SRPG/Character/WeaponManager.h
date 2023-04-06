#pragma once

class WeaponManager : public Singleton<WeaponManager>
{
private:
	friend class Singleton;
	WeaponManager();
	~WeaponManager();
public:
	void Update();
	void Render();

	Weapon* Pop(string key);
	Weapon::WeaponData* GetData(string key);
private:
	void LoadDatas();

private:
	map<string, Weapon::WeaponData> datas;
	map<string, ModelInstancing*> instances;

	vector<Weapon*> weaponPool;
};

