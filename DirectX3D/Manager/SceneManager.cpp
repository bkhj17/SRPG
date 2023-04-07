#include "framework.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	curScenes.clear();
	addScenes.clear();
	removeScenes.clear();
	for (auto& scene : scenes)
		delete scene.second;
	scenes.clear();
}

void SceneManager::Update()
{
	for (auto scene : curScenes)
		scene->Update();

	if (addScenes.size() > 0)
		AddScene();

	if (removeScenes.size() > 0)
		RemoveScene();
}

void SceneManager::PreRender()
{
	for (auto scene : curScenes)
		scene->PreRender();
}

void SceneManager::Render()
{
	for (auto scene : curScenes)
		scene->Render();
}

void SceneManager::PostRender()
{
	for (auto scene : curScenes)
		scene->PostRender();
}

void SceneManager::GUIRender()
{
	for (auto scene : curScenes)
		scene->GUIRender();
}

void SceneManager::Create(string key, Scene* scene)
{
	if (scenes.find(key) != scenes.end())
		return;

	scenes[key] = scene;
}

Scene* SceneManager::Add(string key)
{

	if (scenes.find(key) == scenes.end())
		return nullptr;
		
	addScenes.push_back(key);
	return scenes[key];
}

void SceneManager::Remove(string key)
{
	if (scenes.find(key) == scenes.end())
		return;

	removeScenes.push_back(key);
}

void SceneManager::AddScene()
{
	for (string scene : addScenes) {
		if (scenes.find(scene) == scenes.end())
			continue;

		auto findScene = find(curScenes.begin(), curScenes.end(), scenes[scene]);
		if (findScene != curScenes.end())
			continue;

		curScenes.push_back(scenes[scene]);
		curScenes.back()->Start();
	}

	addScenes.clear();
}

void SceneManager::RemoveScene()
{
	for (string scene : removeScenes) {
		if (scenes.find(scene) == scenes.end())
			continue;

		auto findScene = find(curScenes.begin(), curScenes.end(), scenes[scene]);
		if (findScene == curScenes.end())
			continue;
		
		(*findScene)->End();
		curScenes.erase(findScene);
	}

	removeScenes.clear();
}
