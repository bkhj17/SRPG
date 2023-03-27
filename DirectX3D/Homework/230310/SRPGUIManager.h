#pragma once

class SRPGUIManager : public Singleton<SRPGUIManager>
{
private:
	friend class Singleton;
	SRPGUIManager();
	~SRPGUIManager();
public:
	void Update();
	void Render();

	void SpawnDamage(Vector3 pos, int damage);

private:
	vector<class FloatingDamage*> floatingDamages;
};

