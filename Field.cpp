#include "Include.h"


/// <summary>
/// コンストラクタ
/// </summary>
Field::Field()
{
	//modelHandle = MV1LoadModel("material/mv1/field.mv1");
	//modelHandle = MV1LoadModel("material/mv1/city/city_0525.mv1");
	modelHandle = MV1LoadModel("material/mv1/new_city/0604.mv1");
	boxHandle = MV1LoadModel("material/mv1/new_city/0604_box.mv1");
	meshHandle = MV1LoadModel("material/mv1/new_city/0610_mesh.mv1");

	position = VGet(0, 0, 0);
	//モデルの大きさ調整
	//MV1SetScale(modelHandle, VGet(1.0f, 0.17f, 1.0f));
	//MV1SetScale(modelHandle, VGet(0.05f, 0.05f, 0.05f));
	MV1SetScale(modelHandle, VGet(1.0f, 1.0f, 1.0f));

	MV1SetPosition(modelHandle, position);
	MV1SetPosition(meshHandle, position);
	MV1SetPosition(boxHandle, position);

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
	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(meshHandle, -1, 1, 1, 1);

	int a = MV1GetMaterialNum(meshHandle);

	for (int i = 0; i < a; i++)
	{
		// ３Ｄモデルに含まれる０番目のマテリアルの描画ブレンドモードを DX_BLENDMODE_ADD に変更する
		MV1SetMaterialDrawBlendMode(meshHandle, i, DX_BLENDMODE_ADD);
		// マテリアルのブレンドパラメータを 128 に変更する
		MV1SetMaterialDrawBlendParam(meshHandle, i, 0);
	}
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

	// モデルの０番目のフレームのコリジョン情報を構築
	//MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

}

/// <summary>
/// 描画
/// </summary>
void Field::Draw()
{
	if (isPoly)
	{
		MV1SetWireFrameDrawFlag(modelHandle, TRUE);
		MV1SetWireFrameDrawFlag(meshHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
		MV1SetWireFrameDrawFlag(meshHandle, FALSE);
	}

	MV1DrawModel(modelHandle);
	 
	MV1DrawModel(meshHandle);
}