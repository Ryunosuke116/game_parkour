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
#include "SubSystemManager.h"
#include "EffectManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="modelHandle"></param>
Run::Run(int& modelHandle, 
	AnimState& oldAnimState, AnimState& nowAnimState) :
	PlayerStateBase(modelHandle,  oldAnimState, nowAnimState),
	degree_difference(0.0f),
	stopTime(0.0f),
	angle(-1),
	playerMoveSpeed(-1),
	playerMoveSpeed_max(-1),
	degree_new(-1)
{
	animationNum_now = animNum::run;
	this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
}

/// <summary>
/// デストラクタ
/// </summary>
Run::~Run()
{
}

std::pair<VECTOR, PlayerData> Run::Update(const VECTOR& cameraDirection,
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects, Player& player)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);

	if (playerData.isRunWall)
	{
		auto [moveDir_new,playerData_new] = Update_wallRun(player, fieldObjects);
	
		moveDir = moveDir_new;
		playerData = playerData_new;
	}
	else
	{
		auto [moveDir_new, playerData_new] = Update_normal(cameraDirection, player);
	
		moveDir = moveDir_new;
		playerData = playerData_new;
	}


	if (playerData.isDash)
	{
		const VECTOR scale = VGet(5.0f, 5.0f, 5.0f);
		const auto effectManager = SubSystemManager::GetInstance().GetSubSystem<EffectManager>().lock();
		effectManager->SetScale(scale, "foot_smoke");
		this->nowAnimState.PlayAnimSpeed = DashAnimSpeed;
	}
	else
	{
		this->nowAnimState.PlayAnimSpeed = playAnimSpeed;
	}

	return std::make_pair(moveDir, playerData);
}

/// <summary>
/// 通常時の更新処理
/// </summary>
/// <param name="cameraDirection"></param>
/// <param name="player"></param>
/// <returns></returns>
std::pair<VECTOR,PlayerData> Run::Update_normal(const VECTOR& cameraDirection, Player& player )
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
std::pair<VECTOR, PlayerData> Run::Update_wallRun(
	Player& player, 
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects)
{
	PlayerData playerData = player.GetData();

	VECTOR moveDir = VGet(0.0f, 0.0f, 0.0f);
	const float wallRun_stopTime = player.playerCalculation->GetWallRun_stopTime();
	const float wallRun_stopTime_max = player.playerCalculation->GetWallRun_stopTime_max();

	//落ちる
	//壁がない場合
	if (-PadInput::GetJoyPad_old_y_left() <= -1000 ||
		!playerData.isPossibleWallRun)
	{
		playerData.isRunWall = false;
		playerData.isUseWallJump = false;
		playerData.isFalling = true;
		isChangeState = true;
		player.playerCalculation->Reset_run_wall();
		player.playerCalculation->ResetMoveVec_old();
		player.SetRotata_x(0.0f);

		return std::make_pair(moveDir, playerData);
	}

	//止まって一定時間過ぎたら落下する
	if (wallRun_stopTime >= wallRun_stopTime_max)
	{
		isChangeState = true;
		playerData.isFalling = true;
		playerData.isUseWallJump = false;
		player.SetRotata_x(0.0f);
	}

	//見直し
   //崖掴み判定
	if (playerData.isUseHanging)
	{
		auto result_cliff = HitCheck::CliffGrabbing(
			fieldObjects,
			player.GetPosition(),
			player.GetPositionData().rayTopPosition,
			player.GetNowMoveDirection(),
			cliff_radius);

		//掴むところが平行だった場合
		//崖掴み時の情報を保存
		if (result_cliff.isHitHanging)
		{
			//胴体座標
			VECTOR centerPosition = MV1GetFramePosition(modelHandle, 2);

			Calculation::NearestResult nearestResult = Calculation::SphereMeshOutsideTriangle_line(result_cliff.hangingPoly, centerPosition);

			float difference_y = nearestResult.linePos_start.y - nearestResult.linePos_end.y;

			if (difference_y == 0)
			{
				playerData.isHanging = result_cliff.isHitHanging;
				isChangeState = true;
				player.playerCalculation->SetNearestResult(nearestResult);
				return std::make_pair(moveDir, playerData);
			}
		}
	}

	JumpMove(playerData, player);

	//壁ジャンプする
	if (playerData.isJumpSecond)
	{
		playerData.isRunWall = false;
		playerData.isUseWallJump = true;
		playerData.isWalljump = true;
		//player.SetNowMoveDirection(moveDirection_new);
		player.playerCalculation->Reset_run_wall();

		VECTOR hitWallNormal = player.playerCalculation->GethitWallNormal();
		moveDir = VScale(hitWallNormal, 1.0f);
		player.SetNowMoveDirection(moveDir);
		player.SetRotata_x(0.0f);
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

	if (nowAnimState.AttachIndex != -1)
	{
		// アタッチしたアニメーションの総再生時間を取得する
		totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, nowAnimState.AttachIndex);

		//再生時間更新
		nowAnimState.PlayTime_anim += nowAnimState.PlayAnimSpeed;

		//総再生時間を超えたらリセット
		if (nowAnimState.PlayTime_anim >= totalTime_anim)
		{
			nowAnimState.PlayTime_anim = static_cast<float>(fmod(nowAnimState.PlayTime_anim, totalTime_anim));
		}

		// 再生時間をセットする
		MV1SetAttachAnimTime(modelHandle, nowAnimState.AttachIndex, nowAnimState.PlayTime_anim);

		//アニメーションのモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, nowAnimState.AttachIndex, animBlendRate);
	}


	//再生しているアニメーション２の処理
	if (oldAnimState.AttachIndex != -1)
	{
		// アニメーションの総時間を取得
		totalTime_anim = MV1GetAttachAnimTotalTime(modelHandle, oldAnimState.AttachIndex);

		// 変更した再生時間をモデルに反映させる
		MV1SetAttachAnimTime(modelHandle, oldAnimState.AttachIndex, oldAnimState.PlayTime_anim);

		// アニメーション２のモデルに対する反映率をセット
		MV1SetAttachAnimBlendRate(modelHandle, oldAnimState.AttachIndex, 1.0f - animBlendRate);
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
	angle = player.GetAngle();
	playerMoveSpeed = player.playerCalculation->GetMoveSpeed_now();
	playerMoveSpeed_max = player.playerCalculation->GetMoveSpeed_max();

	//moveDirを取得する
	moveDir = Move(cameraDirection, playerData);
	DashMove(playerData);
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
	moveDirection = VAdd(VScale(rightMove, -PadInput::GetJoyPad_x_left()),
		VScale(upMove, -PadInput::GetJoyPad_y_left()));

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

void Run::ObstacleCheck(
	const std::vector<std::weak_ptr<BaseObject>>& fieldObjects,
	const VECTOR& moveDirection,
	const VECTOR& playerPosition,
	const float radius)
{
	const float reverseScale = -1.0f;
	const VECTOR reverseMoveDirection = VScale(moveDirection, reverseScale);

	for (const auto& fieldObject : fieldObjects)
	{

	}
}

void Run::Enter(PlayerData& playerData)
{
	playerData.isRun = true;
	if (playerData.isRunWall)
	{
		playerData.isJumpAll = false;
		playerData.isJumpSecond = false;
	}
}

void Run::Exit(PlayerData& playerData)
{
	playerData.isRun = false;
	playerData.isRunWall = false;
}