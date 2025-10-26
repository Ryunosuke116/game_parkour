#include "Common.h"
#include <memory>
#include <vector>
#include "PlayerStateBase.h"
#include "PlayerData.h"
#include "Run.h"
#include "AnimTime.h"
#include "Player.h"
#include "PadInput.h"
#include "HitCheck.h"
#include "GameInstanceSubSystem.h"
#include "EffectManager.h"
#include "SoundPlayer.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(const int modelHandle) :
	PlayerStateBase(modelHandle)
{
	this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run::~Run()
{
	//処理なし
}

std::pair<VECTOR, PlayerData> Run::Update(
	const VECTOR& cameraDirection,
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
	Player& player)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	if (playerData.isRunWall)
	{
		auto [newMoveDir,newPlayerData] = WallRunUpdate(player, fieldObjects);
	
		moveDir = newMoveDir;
		playerData = newPlayerData;
	}
	else
	{
		auto [newMoveDir, newPlayerData] = NormalUpdate(cameraDirection, player);
	
		moveDir = newMoveDir;
		playerData = newPlayerData;
	}

	return std::make_pair(moveDir, playerData);
}

/// <summary>
/// 通常時の更新処理
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR,PlayerData> Run::NormalUpdate(
	const VECTOR& cameraDirection,
	Player& player)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = Command(cameraDirection, playerData, player);

	if (!playerData.isGround)
	{
		playerData.isFalling = true;
		isChangeState = true;
	}

	return std::make_pair(moveDir, playerData);
}

/// <summary>
/// 壁走り時の更新処理
/// </summary>
/// <param name="player"></param>
/// <param name="fieldObjects"></param>
/// <returns></returns>
std::pair<VECTOR, PlayerData> Run::WallRunUpdate(
	Player& player, 
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);
	const float wallRunStopTime = player.playerCalculation->GetWallRunStopTime();
	const float wallRunMaxStopTime = player.playerCalculation->GetWallRunMaxStopTime();
	const int kMinJoyPadLeft = 1000;

	//落ちる
	if (PadInput::GetJoyPadYLeft() >= kMinJoyPadLeft &&
		!(PadInput::GetOldJoyPadYLeft() >= kMinJoyPadLeft))
	{
		playerData.isRunWall = false;
		playerData.isUseWallJump = false;
		playerData.isFalling = true;
		isChangeState = true;
		player.playerCalculation->ResetWallRun();
		player.SetRotateX(0.0f);

		return std::make_pair(moveDir, playerData);
	}

	//止まって一定時間過ぎたら落下する
	if (wallRunStopTime >= wallRunMaxStopTime)
	{
		isChangeState = true;
		playerData.isFalling = true;
		playerData.isUseWallJump = false;
		player.SetRotateX(0.0f);

		return std::make_pair(moveDir, playerData);
	}

   //崖掴み判定
	if (playerData.isUseHanging)
	{
		VECTOR distancePlayerTopAndBottom =
			VSub(player.GetPositionData().rayTopPosition,
				player.GetPositionData().rayBottomPosition);

		float playerSize = VSize(distancePlayerTopAndBottom);
		VECTOR normalPlayerTopPosition = player.GetPositionData().rayBottomPosition;
		normalPlayerTopPosition.y += playerSize;

		auto resultCheckCliff = HitCheck::CliffGrabbing(
			fieldObjects,
			player.GetPosition(),
			normalPlayerTopPosition,
			player.GetFaceDirection(),
			kCliffRadius);

		//掴むところが平行だった場合
		//崖掴み時の情報を保存
		if (resultCheckCliff.isHitHanging)
		{
			//胴体座標
			const int kChestBoneNumber = MV1SearchFrame(modelHandle,
				"mixamorig:Spine1");
			VECTOR centerPosition = MV1GetFramePosition(modelHandle, kChestBoneNumber);

			NearestResult nearestResult =
				Calculation::SphereMeshOutsideTriangleLine(
				resultCheckCliff.hangingPoly,
				centerPosition);

			playerData.isHanging = resultCheckCliff.isHitHanging;
			playerData.isMove = false;
			isChangeState = true;
			player.playerCalculation->SetNearestResult(nearestResult);
			player.SetRotateX(0.0f);
			
			return std::make_pair(moveDir, playerData);
		}
	}

	JumpMove(playerData, player);

	//壁ジャンプする
	if (playerData.isSecondJump)
	{
		playerData.isRunWall = false;
		playerData.isUseWallJump = true;
		playerData.isWalljump = true;
		player.playerCalculation->ResetWallRun();

		VECTOR hitWallNormal = player.playerCalculation->GetWallRunGravity();
		moveDir = hitWallNormal;
		player.SetNowMoveDirection(moveDir);
		player.SetRotateX(0.0f);
	}

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
VECTOR Run::Command(
	const VECTOR& cameraDirection,
	PlayerData& playerData,
	Player& player)
{
	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	//moveDirを取得する
	moveDir = Move(cameraDirection, playerData);
	JumpMove(playerData, player);
	RollMove(playerData);

	//急転回せずに止まる場合
	if (!playerData.isMove &&
		!playerData.isRoll &&
		!playerData.isJump)
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
VECTOR Run::Move(
	const VECTOR& cameraDirection,
	PlayerData& playerData)
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
	if (VSize(moveDirection) >= 1e-4f)
	{
		moveDirection = VNorm(moveDirection);
		playerData.isMove = true;
	}
	else
	{
		playerData.isMove = false;
	}

	//必ず正規化されたものか0を返す
	return moveDirection;
}

void Run::Enter(const AnimState& oldAnimState, 
	const AnimState& nowAnimState)
{
	PlayerStateBase::Enter(oldAnimState, nowAnimState);
	const auto soundPlayer =
		GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
	soundPlayer->Play("dash");
}

void Run::Exit(PlayerData& playerData)
{
	PlayerStateBase::Exit(playerData);
	playerData.isRun = false;
	playerData.isRunWall = false;
	const auto soundPlayer =
		GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

	soundPlayer->Stop("dash");
}