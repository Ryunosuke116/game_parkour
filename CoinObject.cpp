
#include "EffectManager.h"
#include "CoinObject.h"

/// @brief コンストラクタ
/// @param path 
CoinObject::CoinObject(const int& handle, const VECTOR& pos):
	radian_Y(0.0f),
	isHitPlayer(false),
	deleteFlag(false),
	hitFlag(false)
{
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
	velocity_Y = 0.0f;
	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(objectScale, objectScale, objectScale));

}

/// @brief デストラクタ
CoinObject::~CoinObject()
{

}

/// @brief 初期化
void CoinObject::Initialize()
{
	position = VGet(5.0f, 8.0f, 5.0f);
	velocity_Y = 0.0f;
	MV1SetPosition(modelHandle, position);
	radian_Y = 0.0f;
	isHitPlayer = false;
	deleteFlag = false;
	hitFlag = false;
}

void CoinObject::Update(){}

/// @brief 更新
bool CoinObject::Update(std::shared_ptr<EffectManager>& effectManager,
	const VECTOR& playerpos_top,
	const VECTOR& playerPos_bottom,const float radius)
{
	VECTOR nearCapsulePos = HitCheck::CapsuleHitConfirmation(playerpos_top, playerPos_bottom, position, radius, 4.5f);

	if (HitCheck::HitConfirmation(position, nearCapsulePos, 4.5f, radius))
	{
		hitFlag = true;
	}

	Rotate();

	if (hitFlag)
	{
		HitPlayerAction();
	}

	return deleteFlag;
}

/// @brief 描画
bool CoinObject::Draw()
{
	//printfDx("coin: %d", hitFlag);
	MV1DrawModel(modelHandle);
	//DrawSphere3D(position, 4.5f, 5, GetColor(0, 0, 0), GetColor(255, 0, 0), FALSE);
	return true;
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