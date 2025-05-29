#include "Include.h"

/// <summary>
/// インスタンス化
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
	player->Initialize();
	camera->Initialize();
	map->Initialize();
	field->Initialize();
	lightPos = VGet(0, 30, 3000);
}

/// <summary>
/// 更新
/// </summary>
void Game::Update()
{
	player->Update(camera->GetCameraDirection());
	camera->Update(player->GetPosition());
	map->Update();
	field->Update();
	collisionManager->Update(*player, field->GetModelHandle());

}

void Game::Draw()
{
	clsDx();
	player->Draw();
	camera->Draw();
	map->Draw();
	field->Draw();
	collisionManager->Draw();
	printfDx("LightPos.x %f\n", GetLightPosition().x);
	printfDx("LightPos.y %f\n", GetLightPosition().y);
	printfDx("LightPos.z %f\n", GetLightPosition().z);
	printfDx("LightDir.x %f\n", GetLightDirection().x);
	printfDx("LightDir.y %f\n", GetLightDirection().y);
	printfDx("LightDir.z %f\n", GetLightDirection().z);
	
	/*printfDx("hitCheck %d\n", hitCheck->HitRayJudge(field->GetModelHandle(), -1,
		player->GetCenterPos(), player->GetFootPos()));*/
}