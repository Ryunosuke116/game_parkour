#include "Include.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	objectManager = std::make_shared<ObjectManager>();
	objectManager->Create();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Game::~Game()
{

}

/// <summary>
/// ������
/// </summary>
void Game::Initialize()
{
	objectManager->Initialize();
}

/// <summary>
/// �X�V
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