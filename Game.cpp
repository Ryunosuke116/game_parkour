#include "common.h"
#include "EffekseerForDXLib.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "Game.h"
#include "DebugDrawer.h"
#include "HitCheck.h"


/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) : BaseScene{ manager }
{
	gameObjectManager = std::make_shared<GameObjectManager>();
	gameObjectManager->Create();
	gameObjectManager_actual = std::dynamic_pointer_cast<GameObjectManager>(gameObjectManager);
}

/// <summary>
/// デストラクタ
/// </summary>
Game::~Game()
{

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

	}
}

void Game::Draw()
{
	clsDx();
	gameObjectManager->Draw();
	DebugDrawer::Instance().Draw();
}