#include "EffectManager.h"
#include "CoinObject.h"

#include "SoundPlayer.h"
#include "SubSystemManager.h"
#include <cassert>

/// <summary>
/// コンストラクタ
/// </summary>
CoinObject::CoinObject():
	BaseObject(),
	radianY(0.0f),
	velocity_Y(0.0f),
	flyAwayVelocity(VGet(0.0f, 0.0f, 0.0f)),
	flyAwayDirection(VGet(0.0f, 0.0f, 0.0f)),
	boundsMin(VGet(0.0f, 0.0f, 0.0f)),
	boundsMax(VGet(0.0f, 0.0f, 0.0f)),
	isHitPlayer(false),
	isDelete(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
CoinObject::~CoinObject()
{

}

/// <summary>
/// 情報読み込み
/// コインをマネージャーで量産する前提
/// </summary>
/// <param name="handle"></param>
/// <param name="pos"></param>
void CoinObject::Load(
	const int listNumber,
	const int handle, 
	const VECTOR& pos)
{
	this->listNumber = listNumber;
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
	MV1SetScale(modelHandle, VGet(objectScale, objectScale, objectScale));
}

/// <summary>
/// 初期化
/// </summary>
void CoinObject::Initialize()
{
	const VECTOR addAABB = VGet(radius, radius, radius);	//AABBの領域範囲

	MV1SetPosition(modelHandle, position);
	velocity_Y = 0.0f;
	radianY = 0.0f;
	isHitPlayer = false;
	isDelete = false;
	isSound = false;
	boundsMin = VSub(position, addAABB);
	boundsMax = VAdd(position, addAABB);
}


/// <summary>
/// 更新
/// </summary>
/// <param name="effectManager"></param>
/// <param name="topPlayerPos"></param>
/// <param name="bottomPlayerPos"></param>
/// <param name="playerRadius"></param>
/// <returns></returns>
bool CoinObject::Update(
	const VECTOR& topPlayerPos,
	const VECTOR& bottomPlayerPos,
	const float playerRadius)
{
	Rotate();

	if (isHitPlayer)
	{
		HitPlayerAction();
	}

	return isDelete;
}

/// <summary>
/// 更新
/// </summary>
void CoinObject::Update()
{
	Rotate();

	if (isHitPlayer)
	{
		HitPlayerAction();
	}
}

/// <summary>
/// 描画
/// </summary>
void CoinObject::Draw()
{
	MV1DrawModel(modelHandle);
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void CoinObject::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void CoinObject::ResultInitialize()
{
	flyAwayDirection = VGet(0.0f, 0.0f, 1.0f);
	flyAwayVelocity = VGet(0.0f, 0.0f, 0.0f);

	flyAwayVelocity = VAdd(flyAwayVelocity, flyAwayDirection);
	flyAwayVelocity.y += flyPower;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void CoinObject::ResultUpdate()
{
	const float gravity = -0.2f;

	position = VAdd(position, flyAwayVelocity);
	flyAwayVelocity.y += gravity;

	radianY += 20.0f;

	//コインモデルを回転させる
	if (radianY >= 360.0f)
	{
		radianY = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radianY * DX_PI_F / 180.0f, 0.0f));
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// プレイヤーと接触した時
/// </summary>
/// <param name="playerPos"></param>
void CoinObject::HitPlayerAction()
{
	const float maxVelocityY = 8.0f;

	if (velocity_Y <= maxVelocityY)
	{
		velocity_Y += addVelocity;
		position.y += addVelocity;
	}
	else
	{
		isDelete = true;
	}

	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// コインモデルの回転制御
/// </summary>
void CoinObject::Rotate()
{
	const float normalAddRadianY = 1.0f;
	const float addRadianY = 20.0f;

	if (!isHitPlayer)
	{
		radianY += normalAddRadianY;
	}
	else
	{
		radianY += addRadianY;
	}

	//コインモデルを回転させる
	if (radianY >= 360.0f)
	{
		radianY = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radianY * DX_PI_F / 180.0f, 0.0f));
}

/// <summary>
/// プレイヤーとの接触判定
/// </summary>
/// <param name="topPlayerPos"></param>
/// <param name="bottomPlayerPos"></param>
/// <param name="playerRadius"></param>
/// <returns></returns>
bool CoinObject::IsHitPlayer(const VECTOR& topPlayerPos,
	const VECTOR& bottomPlayerPos,
	const float playerRadius)
{
	//既に当たっていたら戻る
	if (isHitPlayer)return false;

	//対象の座標から最も近いカプセルの軸座標を算出
	VECTOR nearCapsulePos = HitCheck::CapsuleHitConfirmation(topPlayerPos, bottomPlayerPos, position);

	//プレイヤーと接触しているか
	if (HitCheck::HitConfirmation(position, nearCapsulePos, radius, playerRadius))
	{
		isHitPlayer = true;
		if (!isSound)
		{
			const auto& soundPlayer = SubSystemManager::GetInstance().GetSubSystem<SoundPlayer>().lock();
			soundPlayer->Play("coinGet");
			isSound = true;
		}
		return true;
	}

	return false;
}

