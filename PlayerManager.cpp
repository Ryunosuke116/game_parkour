#include "common.h"
#include "Player.h"
#include "CollisionManager.h"
#include "PlayerManager.h"
#include "Calclation.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager()
{
	collisionManager = std::make_shared<CollisionManager>();
	player = std::make_shared<Player>();
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

/// <summary>
/// 初期化
/// </summary>
void PlayerManager::Initialize()
{
	player->Initialize();
}

/// <summary>
/// 更新
/// </summary>
/// <param name="mapHandle"></param>
/// <param name="player"></param>
void PlayerManager::Update(int mapHandle, const VECTOR& cameraDirection)
{

	actualPlayer->Update(cameraDirection, mapHandle);

	/*if(actualPlayer->GetData().isHangring)
	{
		actualPlayer->SetPos(PositionCheck(actualPlayer->GetHangringPoint(), actualPlayer->GetPosition()));
	}*/

	// プレイヤーのモデルの座標を更新する
	MV1SetPosition(actualPlayer->GetModelHandle(), actualPlayer->GetPosition());
}

/// <summary>
/// 描画
/// </summary>
bool PlayerManager::Draw()
{
	player->Draw();
	collisionManager->Draw();
	return true;
}

VECTOR PlayerManager::PositionCheck(const VECTOR& hangringPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hangring = hangringPos;
	hangring.y = playerPos.y;

	Calclation::Leap(newPlayerPos, hangring, 0.1f);

	return newPlayerPos;
}