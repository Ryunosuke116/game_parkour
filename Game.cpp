#include "common.h"
#include "GameObjectManager.h"
#include "SceneManager.h"
#include "BaseScene.h"
#include "Game.h"

/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	gameObjectManager = std::make_shared<GameObjectManager>();
	gameObjectManager->Create();
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
}

void Game::Draw()
{
	clsDx();
	gameObjectManager->Draw();
}