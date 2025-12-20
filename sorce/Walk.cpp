#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PlayerData.h"
#include "Walk.h"
#include "AnimNum.h"
#include "Player.h"
#include "PadInput.h"
#include "HitCheck.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Walk::Walk(const int modelHandle) :
	PlayerStateBase(modelHandle)
{
	this->nowAnimState.playAnimSpeed = kPlayAnimSpeed;
	animBlendRate = 1.0f;
}

/// <summary>
/// デストラクタ
/// </summary>
Walk::~Walk()
{
	//処理なし
}

std::pair<VECTOR, PlayerData> Walk::Update(const VECTOR& cameraDirection,
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	return std::make_pair(moveDir, playerData);
}

/// <summary>
/// 行動入力
/// 移動方向
/// ジャンプ
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="playerData"></param>
/// <param name="player"></param>
/// <returns></returns>
VECTOR Walk::Command(const VECTOR& cameraDirection, PlayerData& playerData, Player& player)
{
	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	return moveDir;
}
/// <summary>
/// 移動方向入力
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="playerData"></param>
/// <returns></returns>
VECTOR Walk::Move(const VECTOR& cameraDirection, PlayerData& playerData)
{
	VECTOR moveDirection = VGet(0.0f, 0.0f, 0.0f);

	return moveDirection;
}

void Walk::Exit(PlayerData& playerData)
{
	PlayerStateBase::Exit(playerData);
	playerData.isWalk = false;
}