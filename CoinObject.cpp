
#include "CoinObject.h"

/// @brief コンストラクタ
/// @param path 
CoinObject::CoinObject(const char* path):
	radian_Y(0.0f)
{
	modelHandle = MV1LoadModel(path);
	position = VGet(0.0f, 0.0f, 0.0f);
	MV1SetScale(modelHandle, VGet(0.1f, 0.1f, 0.1f));

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
}

void CoinObject::Update(){}

/// @brief 更新
void CoinObject::Update(const VECTOR& playerpos_top,const VECTOR& playerPos_bottom,const float radius)
{
	VECTOR nearCapsulePos = hitCheck.CapsuleHitConfirmation(playerpos_top, playerPos_bottom, position, radius, 4.5f);

	hitFlag = hitCheck.HitConfirmation(position, nearCapsulePos, 4.5f, radius);
	
	if (radian_Y >= 360.0f)
	{
		radian_Y = 0.0f;
	}

	radian_Y += 1.0f;

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, radian_Y * DX_PI_F / 180.0f, 0.0f));
}

/// @brief 描画
void CoinObject::Draw()
{
	printfDx("coin: %d", hitFlag);
	MV1DrawModel(modelHandle);
	DrawSphere3D(position, 4.5f, 5, GetColor(0, 0, 0), GetColor(255, 0, 0), FALSE);
}

