#pragma once
class SceneManager : public Singleton<SceneManager>
{
private:
	friend class Singleton;
	SceneManager();
	~SceneManager();
public:
	void Update();

	void PreRender();
	void Render();
	void PostRender();
	void GUIRender();

	void Start() {}
	void End() {}

	void Create(string key, Scene* scene);
	Scene* Add(string key);		//등록된 씬 중 key에 해당하는 것을 curScenes에 삽입
	void Remove(string key);	//curScenes에 key에 해당하는 씬이 있다면 뺀다

private:
	void AddScene();		//등록된 씬 중 key에 해당하는 것을 curScenes에 삽입
	void RemoveScene();	//curScenes에 key에 해당하는 씬이 있다면 뺀다

private:
	unordered_map<string, Scene*> scenes;
	list<Scene*> curScenes;

	vector<string> addScenes, removeScenes;
};
