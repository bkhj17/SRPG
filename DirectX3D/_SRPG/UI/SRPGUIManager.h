#pragma once

class UIWindow;
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

	void CloseAll();

	UIWindow* OpenUI(string key);
	UIWindow* OpenUI(string key, Vector3 pos);
	void CloseUI();
	void CloseUI(string key);

	bool IsActing();
	bool IsMapControl();
private:
	void SetInfo(void* characterPtr);
private:
	vector<class FloatingDamage*> floatingDamages;

	unordered_map<string, UIWindow*> totalUI;
	list<UIWindow*> openned;
};
