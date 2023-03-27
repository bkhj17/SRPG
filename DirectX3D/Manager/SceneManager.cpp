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

	if (addScene.length() > 0)
		AddScene();

	if (removeScene.length() > 0)
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
		
	addScene = key;
	return scenes[key];
}

void SceneManager::Remove(string key)
{
	if (scenes.find(key) == scenes.end())
		return;

	removeScene = key;
}

void SceneManager::AddScene()
{
	auto findScene = find(curScenes.begin(), curScenes.end(), scenes[addScene]);
	if (findScene != curScenes.end())
		return;

	curScenes.push_back(scenes[addScene]);
	curScenes.back()->Start();

	addScene = "";
}

void SceneManager::RemoveScene()
{
	if (scenes.find(removeScene) == scenes.end())
		return;

	auto findScene = find(curScenes.begin(), curScenes.end(), scenes[removeScene]);
	if (findScene != curScenes.end()) {
		(*findScene)->End();
		curScenes.erase(findScene);
	}

	removeScene = "";
}
