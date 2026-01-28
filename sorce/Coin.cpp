#include "EffectManager.h"
#include "Coin.h"
#include "SoundPlayer.h"
#include "GameInstanceSubSystem.h"
#include <cassert>
#include "DebugDrawer.h"
#include "Calculation.h"
#include "HitCheck.h"
#include "WorldSubSystem.h"
#include "PlayerManager.h"
#include "Player.h"

/// <summary>
/// コンストラクタ
/// </summary>
Coin::Coin():
	BaseObject(),
	listNumber			(-1),
	radianY				(0.0f),
	velocityY			(0.0f),
	flyAwaySpeed		(0.0f),
	flyAwayVelocity		(VGet(0.0f, 0.0f, 0.0f)),
	flyAwayDirection	(VGet(0.0f, 0.0f, 0.0f)),
	boundsMin			(VGet(0.0f, 0.0f, 0.0f)),
	boundsMax			(VGet(0.0f, 0.0f, 0.0f)),
	isHitPlayer			(false),
	isDelete			(false),
	isSound				(false),
	isIgnoreHitPlayer	(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Coin::~Coin()
{
	MV1DeleteModel(modelHandle);
}

/// <summary>
/// 情報読み込み
/// コインをマネージャーで量産する前提
/// </summary>
/// <param name="handle"></param>
/// <param name="pos"></param>
void Coin::Load(
	const int listNumber,
	const int handle, 
	const VECTOR& pos)
{
	this->listNumber = listNumber;
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
	MV1SetScale(modelHandle, VGet(kObjectScale, kObjectScale, kObjectScale));
}

/// <summary>
/// 初期化
/// </summary>
void Coin::Initialize()
{
	const VECTOR addAABB = VGet(kRadius, kRadius, kRadius);	//AABBの領域範囲

	MV1SetPosition(modelHandle, position);
	velocityY			= 0.0f;
	radianY				= 0.0f;
	flyAwaySpeed		= -2.0f;
	isHitPlayer			= false;
	isDelete			= false;
	isSound				= false;
	isIgnoreHitPlayer	= false;
	boundsMin			= VSub(position, addAABB);
	boundsMax			= VAdd(position, addAABB);
}

/// <summary>
/// 更新
/// </summary>
/// <param name="effectManager"></param>
/// <param name="topPlayerPos"></param>
/// <param name="bottomPlayerPos"></param>
/// <param name="playerRadius"></param>
/// <returns></returns>
bool Coin::Update(
	const VECTOR& topPlayerPos,
	const VECTOR& bottomPlayerPos,
	const float playerRadius)
{
	Rotate();

	if (isHitPlayer)
	{
		HitPlayerAction(topPlayerPos,
			bottomPlayerPos,
			playerRadius);
	}

	return isDelete;
}

/// <summary>
/// 描画
/// </summary>
void Coin::Draw()
{
	if (!CheckCameraViewClip(position))
	{
		MV1DrawModel(modelHandle);
	}
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Coin::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Coin::ResultInitialize()
{
	const VECTOR kInitFlyAwayDirection	= VGet(0.0f, 0.0f, 1.0f);
	const VECTOR kInitFlyAwayVelocity	= VGet(0.0f, 0.0f, 0.0f);

	flyAwayDirection = kInitFlyAwayDirection;
	flyAwayVelocity  = kInitFlyAwayVelocity;

	flyAwayVelocity	   = VAdd(flyAwayVelocity, flyAwayDirection);
	flyAwayVelocity.y += kFlyPower;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Coin::ResultUpdate()
{
	const float kGravity	= -0.2f;
	const float kAddRadian	= 20.0f;

	position = VAdd(position, flyAwayVelocity);
	flyAwayVelocity.y += kGravity;

	radianY += kAddRadian;

	//コインモデルを回転させる
	if (radianY >= 360.0f)
	{
		radianY = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, (radianY * DX_PI_F / 180.0f), 0.0f));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// プレイヤーと接触した時
/// </summary>
/// <param name="playerPos"></param>
void Coin::HitPlayerAction(const VECTOR& topPlayerPos,
	const VECTOR& bottomPlayerPos,
	const float playerRadius)
{
	const float kAddFlyAwaySpeed = 0.1f;
	const float kMaxVelocityY	 = 8.0f;
	const auto& spPlayerManager  = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>();
	
	VECTOR toPlayer = VSub(spPlayerManager->GetPlayer()->GetPositionData().centerPosition, position);

	if(!isIgnoreHitPlayer)
	{
		toPlayer.y = 0.0f;
	}

	toPlayer		= VNorm(toPlayer);
	flyAwayVelocity = VScale(toPlayer, flyAwaySpeed);
	flyAwaySpeed	+= kAddFlyAwaySpeed;

	position = VAdd(position, flyAwayVelocity);

	//対象の座標から最も近いカプセルの軸座標を算出
	VECTOR nearCapsulePos = Calculation::ProjectionDirection(topPlayerPos, bottomPlayerPos, position);

	//プレイヤーと接触しているか
	if (isIgnoreHitPlayer &&
		HitCheck::HitConfirmation(position, nearCapsulePos, kRadius, playerRadius))
	{
		isDelete = true;
	}

	//playerと最初に接触したときに一定時間、playerとの当たり判定を無効化しているため
	//少し経ったら再度当たれるようにする
	if (flyAwaySpeed > 0.0f &&
		!isIgnoreHitPlayer)
	{
		isIgnoreHitPlayer = true;
	}

	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// コインモデルの回転制御
/// </summary>
void Coin::Rotate()
{
	const float kNormalAddRadianY	= 1.0f;
	const float kAddRadianY			= 20.0f;

	if (!isHitPlayer)
	{
		radianY += kNormalAddRadianY;
	}
	else
	{
		radianY += kAddRadianY;
	}

	//コインモデルを回転させる
	if (radianY >= 360.0f)
	{
		radianY = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, (radianY * DX_PI_F / 180.0f), 0.0f));
}

/// <summary>
/// プレイヤーとの接触判定
/// </summary>
/// <param name="topPlayerPos"></param>
/// <param name="bottomPlayerPos"></param>
/// <param name="playerRadius"></param>
/// <returns></returns>
bool Coin::IsHitPlayer(const VECTOR& topPlayerPos,
	const VECTOR& bottomPlayerPos,
	const float playerRadius)
{
	//既に当たっていたら戻る
	if (isHitPlayer)return false;

	//対象の座標から最も近いカプセルの軸座標を算出
	VECTOR nearCapsulePos = Calculation::ProjectionDirection(topPlayerPos, bottomPlayerPos, position);

	//プレイヤーと接触しているか
	if (HitCheck::HitConfirmation(position, nearCapsulePos, kRadius, playerRadius))
	{
		isHitPlayer = true;
		if (!isSound)
		{
			const auto& soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();
			soundPlayer->Play("coinGet");
			isSound = true;
		}
		return true;
	}

	return false;
}
