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
	hitCheck = std::make_shared<HitCheck>();
	collisionManager = std::make_shared<CollisionManager>();
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
	field->Initialize();
}

/// <summary>
/// �X�V
/// </summary>
void Game::Update()
{
	collisionManager->Update(*player, field->GetModelHandle());
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
	/*printfDx("hitCheck %d\n", hitCheck->HitRayJudge(field->GetModelHandle(), -1,
		player->GetCenterPos(), player->GetFootPos()));*/
}