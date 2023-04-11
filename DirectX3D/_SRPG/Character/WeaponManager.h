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

	string GetTypeName(Weapon::Type type);
	string GetTypeName(Weapon* weapon);

	void SetShader(wstring shader);
private:
	void LoadDatas();

private:
	map<string, Weapon::WeaponData> datas;
	map<string, ModelInstancing*> instances;

	vector<Weapon*> weaponPool;

	map<Weapon::Type, string> typeString;
};

