#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "ISubSystem.h"
#include "SubSystemManager.h"

SubSystemManager& SubSystemManager::GetInstance()
{
	static SubSystemManager instance;
	return instance;
}

void SubSystemManager::Create(const std::string& sceneName)
{
	for (auto& system : subSystems)
	{
		system.second->Create(sceneName);
	}
}

void SubSystemManager::Update()
{
	for (auto& system : subSystems)
	{
		system.second->Update();
	}
}

void SubSystemManager::Draw()
{
	for (auto& system : subSystems)
	{
		system.second->Draw();
	}
}

void SubSystemManager::ShutdownAll()
{
	for (auto& pair : subSystems)
	{
		pair.second->Shutdown();
	}
	subSystems.clear();
}