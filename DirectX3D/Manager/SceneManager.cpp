#include "framework.h"
#include "SceneManager.h"

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
	for (auto& scene : scenes)
		delete scene.second;
}

void SceneManager::Update()
{
	for (auto scene : curScenes)
		scene->Update();
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

	auto findScene = find(curScenes.begin(), curScenes.end(), scenes[key]);
	if (findScene != curScenes.end())
		return *findScene;

	curScenes.push_back(scenes[key]);
	curScenes.back()->Start();
	return scenes[key];
}

void SceneManager::Remove(string key)
{
	if (scenes.find(key) == scenes.end())
		return;

	auto findScene = find(curScenes.begin(), curScenes.end(), scenes[key]);
	if (findScene != curScenes.end()) {
		(*findScene)->End();
		curScenes.erase(findScene);
	}
}
