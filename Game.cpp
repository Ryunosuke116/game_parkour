#include "common.h"
#include "EffekseerForDXLib.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "Game.h"
#include "DebugDrawer.h"
#include "HitCheck.h"
#include "SubSystemManager.h"

/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) : BaseScene{ manager }
{
	
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{
	JsonManager::GetInstance().Shutdown();
	SubSystemManager::GetInstance().ShutdownAll();
}

void Game::Create()
{
	const std::string jsonFileName = "JsonGame";
	SubSystemManager::GetInstance().AddSubSystem<SoundPlayer>();
	SubSystemManager::GetInstance().AddSubSystem<EffectManager>();

	JsonManager::GetInstance().Create(jsonFileName);
	SubSystemManager::GetInstance().Create(jsonFileName);

	gameObjectManager = std::make_shared<GameObjectManager>();
	gameObjectManager_actual = std::dynamic_pointer_cast<GameObjectManager>(gameObjectManager);

	gameObjectManager->Create();
	BlackOut::GetInstance().SetAlpha(255);

}

/// <summary>
/// 初期化
/// </summary>
void Game::Initialize()
{
	gameObjectManager->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Game::Update()
{
	PadInput::Update();
	
	gameObjectManager->Update();

	if (gameObjectManager_actual->GetIsGoal())
	{
		ChangeScene("Title", gameObjectManager_actual->GetCoinCount());
	}

}

void Game::StartUpdate()
{
	const int addalpha = 5;
	BlackOut::GetInstance().BlackOutUpdate(addalpha);
	gameObjectManager_actual->StartUpdate();
}

void Game::Draw()
{
	clsDx();
	gameObjectManager->Draw();
	DebugDrawer::Instance().Draw();
}