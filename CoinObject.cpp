#include "CoinObject.h"

/// @brief コンストラクタ
/// @param path 
CoinObject::CoinObject(const char* path)
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
}

/// @brief 更新
void CoinObject::Update()
{

}

/// @brief 描画
void CoinObject::Draw()
{
	MV1DrawModel(modelHandle);
}

