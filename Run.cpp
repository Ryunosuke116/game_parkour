#include "common.h"
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
	PlayerStateBase(modelHandle),
	differenceDegree(0.0f),
	stopTime(0.0f),
	angle(-1),
	playerMoveSpeed(-1),
	playerMoveSpeed_max(-1),
	degree_new(-1)
{
	animationNum_now = animNum::run;
	this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run::~Run()
{
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
		auto [moveDir_new,playerData_new] = WallRunUpdate(player, fieldObjects);
	
		moveDir = moveDir_new;
		playerData = playerData_new;
	}
	else
	{
		auto [moveDir_new, playerData_new] = NormalUpdate(cameraDirection, player);
	
		moveDir = moveDir_new;
		playerData = playerData_new;
	}

	this->nowAnimState.PlayAnimSpeed = kPlayAnimSpeed;

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
	Player& player )
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
	const float WallRunStopTime = player.playerCalculation->GetWallRun_stopTime();
	const float kWallRunMaxStopTime = player.playerCalculation->GetWallRun_stopTime_max();
	const int kMinJoyPadLeft = -1000;

	//落ちる
	if (-PadInput::GetJoyPadYLeft() <= kMinJoyPadLeft)
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
	if (WallRunStopTime >= kWallRunMaxStopTime)
	{
		isChangeState = true;
		playerData.isFalling = true;
		playerData.isUseWallJump = false;
		player.SetRotateX(0.0f);
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

			Calculation::NearestResult nearestResult =
				Calculation::SphereMeshOutsideTriangleLine(
				resultCheckCliff.hangingPoly,
				centerPosition);

			playerData.isHanging = resultCheckCliff.isHitHanging;
			playerData.isMove = false;
			isChangeState = true;
			player.playerCalculation->SetNearestResult(nearestResult);
			
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
/// アニメーション更新
/// </summary>
/// <param name="playerData"></param>
/// <returns></returns>
bool Run::MotionUpdate(PlayerData& playerData)
{
	float totalTime_anim;

	// ブレンド率が１以下の場合は１に近づける
	if (animBlendRate < 1.0f)
	{
		animBlendRate += AnimBlendSpeed;
		if (animBlendRate > 1.0f)
		{
			animBlendRate = 1.0f;
		}
	}

	if (nowAnimState.attachIndex != -1)
	{
		// アタッチしたアニメーションの総再生時間を取得する
		totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.attachIndex);

		//再生時間更新
		nowAnimState.playAnimTime += nowAnimState.PlayAnimSpeed;

		//総再生時間を超えたらリセット
		if (nowAnimState.playAnimTime >= totalTime_anim)
		{
			nowAnimState.playAnimTime = static_cast<float>(fmod(nowAnimState.playAnimTime, totalTime_anim));
		}

		// 再生時間をセットする
		MV1SetAttachAnimTime(modelHandle, nowAnimState.attachIndex, nowAnimState.playAnimTime);

		//アニメーションのモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.attachIndex, animBlendRate);
	}


	//再生しているアニメーション２の処理
	if (oldAnimState.attachIndex != -1)
	{
		// アニメーションの総時間を取得
		totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.attachIndex);

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(modelHandle, oldAnimState.attachIndex, oldAnimState.playAnimTime);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.attachIndex, 1.0f - animBlendRate);
	}

	return false;

	if (!playerData.isGround)
	{
		return true;
	}

	return false;
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
	angle = player.GetRadian();
	playerMoveSpeed = player.playerCalculation->GetMoveSpeed_now();
	playerMoveSpeed_max = player.playerCalculation->GetMoveSpeed_max();

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
	if (!playerData.isMove &&
		!playerData.isRoll)
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

void Run::DashMove(PlayerData& playerData)
{
	PadInput::IsPushLT() ? 
		playerData.isDash = true :
		playerData.isDash = false;
}

void Run::Enter(AnimState& oldAnimState, AnimState& nowAnimState)
{
	PlayerStateBase::Enter(oldAnimState, nowAnimState);
	const auto soundPlayer =
		GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
	soundPlayer->Play("dash");
}

void Run::Exit(PlayerData& playerData)
{
	playerData.isRun = false;
	playerData.isRunWall = false;
	const auto soundPlayer =
		GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

	soundPlayer->Stop("dash");
}