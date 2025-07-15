#include "common.h"
#include "Player.h"
#include "CollisionManager.h"
#include "PlayerManager.h"
#include "Calculation.h"
#include "BaseObject.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerManager::PlayerManager()
{
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerManager::~PlayerManager()
{

}

void PlayerManager::Create() 
{
	player = std::make_shared<Player>(jsonData);
	actualPlayer = std::dynamic_pointer_cast<Player>(player);
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
void PlayerManager::Update(const std::vector<std::shared_ptr<BaseObject>>& fieldObjects ,
	const VECTOR& cameraDirection)
{

	actualPlayer->Update(cameraDirection, fieldObjects);

	/*if(actualPlayer->GetData().isHanging)
	{
		actualPlayer->SetPos(PositionCheck(actualPlayer->GetHangingPoint(), actualPlayer->GetPosition()));
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
	return true;
}

VECTOR PlayerManager::PositionCheck(const VECTOR& hangingPos, const VECTOR& playerPos)
{
	VECTOR newPlayerPos = playerPos;
	VECTOR hanging = hangingPos;
	hanging.y = playerPos.y;

	Calculation::Leap(newPlayerPos, hanging, 0.1f);

	return newPlayerPos;
}

void PlayerManager::Update() {}
void PlayerManager::Add()	 {}