#include "Include.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager()
{
	collisionManager = std::make_shared<CollisionManager>();
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update(int mapHandle, Player& player)
{
	collisionManager->Update(player, mapHandle);
}

/// <summary>
/// 描画
/// </summary>
void PlayerManager::Draw()
{
	collisionManager->Draw();
}