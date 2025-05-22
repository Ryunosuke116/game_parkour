#include "Include.h"

/// <summary>
/// コンストラクタ
/// </summary>
Map::Map()
{
	modelHandle = MV1LoadModel("material/skyDome/sunSet.mv1");
	MV1SetScale(modelHandle, VGet(2.0f, 2.0f, 2.0f));
}

/// <summary>
/// デストラクタ
/// </summary>
Map::~Map()
{

}

/// <summary>
/// 初期化
/// </summary>
void Map::Initialize()
{

}

/// <summary>
/// 更新
/// </summary>
void Map::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void Map::Draw()
{
	MV1DrawModel(modelHandle);
}