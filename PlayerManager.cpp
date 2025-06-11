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
	/*auto result = collisionManager->Update(player, mapHandle);
	player.SetIsGround(result.first);
	player.SetPos(result.second);*/

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(player.GetModelHandle(), player.GetPosition());
}

/// <summary>
/// 描画
/// </summary>
void PlayerManager::Draw()
{
	collisionManager->Draw();
}