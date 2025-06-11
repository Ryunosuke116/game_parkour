#include "Include.h"

/// <summary>
/// インスタンス化
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	objectManager = std::make_shared<ObjectManager>();
	objectManager->Create();
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
	objectManager->Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Game::Update()
{
	objectManager->Update();
}

void Game::Draw()
{
	clsDx();
	objectManager->Draw();
}