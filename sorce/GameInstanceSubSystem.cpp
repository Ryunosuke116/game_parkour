#include <iostream>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include "IGameInstanceSubSystem.h"
#include "GameInstanceSubSystem.h"
#include "assert.h"
#include "SoundPlayer.h"
#include "EffectManager.h"

GameInstanceSubSystem& GameInstanceSubSystem::GetInstance()
{
	static GameInstanceSubSystem instance;
	return instance;
}

void GameInstanceSubSystem::Load(const std::string& sceneName)
{
	for (auto& system : subSystems)
	{
		system.second->Create(sceneName);
	}
}

void GameInstanceSubSystem::Create()
{
	if (subSystems.size() != 0)
	{
		assert("subSystemsの初期化が行われていません");
	}

	//オブジェクトを生成+管理
	GameInstanceSubSystem::GetInstance().AddSubSystem<SoundPlayer>();
	GameInstanceSubSystem::GetInstance().AddSubSystem<EffectManager>();
}

void GameInstanceSubSystem::Update()
{
	for (auto& system : subSystems)
	{
		system.second->Update();
	}
}

void GameInstanceSubSystem::Draw()
{
	for (auto& system : subSystems)
	{
		system.second->Draw();
	}
}

/// <summary>
/// 管理しているオブジェクトのdelete処理
/// </summary>
void GameInstanceSubSystem::ShutdownAll()
{
	for (auto& system : subSystems)
	{
		system.second->Shutdown();
	}
}