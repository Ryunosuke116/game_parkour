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
	radian_Y(0.0f),
	velocity_Y(0.0f),
	isHitPlayer(false),
	deleteFlag(false),
	hitFlag(false)
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
void CoinObject::Load(const int& handle, const VECTOR& pos)
{
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
	MV1SetScale(modelHandle, VGet(objectScale, objectScale, objectScale));
}

/// <summary>
/// 初期化
/// </summary>
void CoinObject::Initialize()
{
	MV1SetPosition(modelHandle, position);
	velocity_Y = 0.0f;
	radian_Y = 0.0f;
	isHitPlayer = false;
	deleteFlag = false;
	hitFlag = false;
	isSound = false;
}


/// <summary>
/// 更新
/// </summary>
/// <param name="effectManager"></param>
/// <param name="playerpos_top"></param>
/// <param name="playerPos_bottom"></param>
/// <param name="radius"></param>
/// <returns></returns>
bool CoinObject::Update(
	const VECTOR& playerpos_top,
	const VECTOR& playerPos_bottom,
	const float radius)
{
	VECTOR nearCapsulePos = HitCheck::CapsuleHitConfirmation(playerpos_top, playerPos_bottom, position, radius, 4.5f);

	if (HitCheck::HitConfirmation(position, nearCapsulePos, 4.5f, radius))
	{
		hitFlag = true;
		if (!isSound)
		{
			const auto& soundPlayer = SubSystemManager::GetInstance().GetSubSystem<SoundPlayer>().lock();
			soundPlayer->Play("coinGet");
			isSound = true;
		}
	}

	Rotate();

	if (hitFlag)
	{
		HitPlayerAction();
	}

	return deleteFlag;
}

/// <summary>
/// 描画
/// </summary>
void CoinObject::Draw()
{
	//printfDx("coin: %d", hitFlag);
	MV1DrawModel(modelHandle);
}

/// <summary>
/// プレイヤーと接触した時
/// </summary>
/// <param name="playerPos"></param>
void CoinObject::HitPlayerAction()
{
	if (velocity_Y <= 8.0f)
	{
		velocity_Y += addVelocity;
		position.y += addVelocity;
	}
	else
	{
		deleteFlag = true;
	}

	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// コインモデルの回転制御
/// </summary>
void CoinObject::Rotate()
{
	if (!hitFlag)
	{
		radian_Y += 1.0f;
	}
	else
	{
		radian_Y += 20.0f;
	}

	//コインモデルを回転させる
	if (radian_Y >= 360.0f)
	{
		radian_Y = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radian_Y * DX_PI_F / 180.0f, 0.0f));
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void CoinObject::ResultInitialize()
{
	flyAwayDirection = VGet(0.0f, 0.0f, 1.0f);

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

	radian_Y += 20.0f;

	//コインモデルを回転させる
	if (radian_Y >= 360.0f)
	{
		radian_Y = 0.0f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radian_Y * DX_PI_F / 180.0f, 0.0f));
	MV1SetPosition(modelHandle, position);
}