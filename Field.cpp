#include "Include.h"


/// <summary>
/// コンストラクタ
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	modelHandle = MV1LoadModel("material/mv1/city/city_floor.mv1");
	position = VGet(0, 0, 0);
	//モデルの大きさ調整
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetPosition(modelHandle, position);

}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field()
{

}

/// <summary>
/// 初期化
/// </summary>
void Field::Initialize()
{
	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
}

/// <summary>
/// 更新
/// </summary>
void Field::Update()
{
	if (CheckHitKey(KEY_INPUT_1))
	{
		if (!isPush)
		{
			if (!isPoly)
			{
				isPoly = true;
			}
			else
			{
				isPoly = false;
			}
			isPush = true;
		}
	}
	else
	{
		isPush = false;
	}

}

/// <summary>
/// 描画
/// </summary>
void Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
	}
	MV1DrawModel(modelHandle);
}