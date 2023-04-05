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
	WeaponData* GetData(string key);
private:
	void LoadDatas();

private:
	map<string, WeaponData> datas;
	map<string, ModelInstancing*> instances;

	vector<Weapon*> weaponPool;
};

