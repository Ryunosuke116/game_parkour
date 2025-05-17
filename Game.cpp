#include "Include.h"

/// <summary>
/// �C���X�^���X��
/// </summary>
/// <param name="manager"></param>
Game::Game(SceneManager& manager) :BaseScene{ manager }
{
	player = std::make_shared<Player>();
	camera = std::make_shared<Camera>();
	map = std::make_shared<Map>();
	input = std::make_shared<Input>();
	field = std::make_shared<Field>();
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
	player->Initialize();
	camera->Initialize();
	map->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void Game::Update()
{
	player->Update(camera->GetCameraDirection());
	camera->Update(player->GetPosition());
	map->Update();
}

void Game::Draw()
{
	clsDx();
	player->Draw();
	camera->Draw();
	map->Draw();
	field->Draw();
}