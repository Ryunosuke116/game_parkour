
#include "CoinObject.h"

/// @brief コンストラクタ
/// @param path 
CoinObject::CoinObject(const int& handle, const VECTOR& pos):
	radian_Y(0.0f),
	isHitPlayer(false)
{
	modelHandle = MV1DuplicateModel(handle);
	position = pos;
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
	MV1SetPosition(modelHandle, position);
	radian_Y = 0.0f;
	isHitPlayer = false;
}

void CoinObject::Update(){}

/// @brief 更新
bool CoinObject::Update(const VECTOR& playerpos_top,const VECTOR& playerPos_bottom,const float radius)
{
	VECTOR nearCapsulePos = HitCheck::CapsuleHitConfirmation(playerpos_top, playerPos_bottom, position, radius, 4.5f);

	hitFlag = HitCheck::HitConfirmation(position, nearCapsulePos, 4.5f, radius);
	
	if (radian_Y >= 360.0f)
	{
		radian_Y = 0.0f;
	}

	radian_Y += 1.0f;

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radian_Y * DX_PI_F / 180.0f, 0.0f));

	return hitFlag;
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
void CoinObject::HitPlayerAction(const VECTOR& playerPos, const float& radius)
{
	//プレイヤーと当たった時、コインを消す
	if (HitCheck::HitConfirmation(position, playerPos, this->radius, radius))
	{
		isHitPlayer = true;
	}
	//消えるときに何かしらのアクションをつけたい
}
