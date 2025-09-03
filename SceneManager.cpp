#include "common.h"
#include <cassert>
#include "BaseScene.h"
#include "SceneManager.h"
#include "SubSystemManager.h"

SceneManager::SceneManager() :nowScene{ nullptr } {}

SceneManager::~SceneManager()
{
	for (const auto& pair : scenes)
	{
		delete pair.second;
	}
	SubSystemManager::GetInstance().ShutdownAll();
}


void SceneManager::ChangeScene(const std::string_view name, const int coinCount)
{
	try
	{
		nowScene->Shutdown();
		nowScene = scenes.at(name);
		nowScene->SetCoinCount(coinCount);
		nowScene->Create();
		nowScene->Initialize();
	}
	catch (...)
	{
		assert(!"存在しないキーが指定されました。");
	}
}

void SceneManager::Initialize()
{
	nowScene->Initialize();
}

void SceneManager::Update()
{
	nowScene->Update();
	SubSystemManager::GetInstance().Update();
}

void SceneManager::Draw()
{
	nowScene->Draw();
	SubSystemManager::GetInstance().Draw();
}

