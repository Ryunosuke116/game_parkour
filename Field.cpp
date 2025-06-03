#include "Include.h"


/// <summary>
/// コンストラクタ
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	//modelHandle = MV1LoadModel("material/mv1/city/city_0525.mv1");
	modelHandle = MV1LoadModel("material/mv1/new_city/0603.mv1");

	position = VGet(0, 0, 0);
	y = 0.0f;
	//モデルの大きさ調整
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	//MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));
	MV1SetRotationXYZ(modelHandle, VGet(0.0f, y * DX_PI_F / 180.0f, 0.0f));

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
	SetLightPosition(VGet(0.0f, -200.0f, 0.0f));
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

	if (CheckHitKey(KEY_INPUT_W))
	{
		y += 0.1f;
	}
	if (CheckHitKey(KEY_INPUT_S))
	{
		y -= 0.1f;
	}

	MV1SetRotationXYZ(modelHandle, VGet(0.0f, y * DX_PI_F / 180.0f, 0.0f));
	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
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