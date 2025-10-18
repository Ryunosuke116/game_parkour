#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PlayerData.h"
#include "Walk.h"
#include "AnimTime.h"
#include "Player.h"
#include "PadInput.h"
#include "HitCheck.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Walk::Walk(const int modelHandle) :
	PlayerStateBase(modelHandle),
	differenceDegree(0.0f),
	stopTime(0.0f),
	angle(-1),
	playerMoveSpeed(-1),
	playerMoveSpeed_max(-1),
	degree_new(-1)
{
	this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
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
	angle = player.GetRadian();
	playerMoveSpeed = player.playerCalculation->GetNowMoveSpeed();
	playerMoveSpeed_max = player.playerCalculation->GetMaxMoveSpeed();

	//moveDirを取得する
	moveDir = Move(cameraDirection, playerData);
	JumpMove(playerData, player);
	RollMove(playerData);

	//前フレームと現在のフレームで入力されてなければ動いてない
	if (stopTime >= 3.0f)
	{
		playerData.isMove = false;
	}
	else
	{
		playerData.isMove = true;
	}

	//急転回せずに止まる場合
	if (!playerData.isMove && !playerData.isRoll)
	{
		playerData.isStopRun = true;
		isChangeState = true;
	}

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

	playerData.isMove = false;
	VECTOR rightMove = VCross(cameraDirection, VGet(0.0f, 1.0f, 0.0f));

	//正規化
	rightMove = VNorm(rightMove);
	VECTOR upMove = VNorm(cameraDirection);

	upMove.y = 0.0f;
	rightMove.y = 0.0f;

	//パッド or arrowキーの入力方向で計算
	moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPadXLeft()),
		VScale(upMove, -PadInput::GetJoyPadYLeft()));

	//0でなければ正規化
	if (VSize(moveDirection) != 0)
	{
		moveDirection = VNorm(moveDirection);
		stopTime = 0.0f;
	}
	else
	{
		stopTime++;
	}

	//3fの間入力されてなければ動いてない
	if (stopTime >= 3.0f)
	{
		playerData.isMove = false;
	}
	else
	{
		playerData.isMove = true;
	}


	//必ず正規化されたものか0を返す
	return moveDirection;

}

void Walk::Exit(PlayerData& playerData)
{
	playerData.isWalk = false;
}