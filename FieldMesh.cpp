#include "FieldMesh.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="path"></param>
FieldMesh::FieldMesh(const char* path):
	isPoly(true),
	isPush(true)
{
	modelHandle = MV1LoadModel(path);
	position = VGet(0, 0, 0);
	MV1SetPosition(modelHandle, position);
}

/// <summary>
/// デストラクタ
/// </summary>
FieldMesh::~FieldMesh()
{

}

/// <summary>
/// 初期化
/// </summary>
void FieldMesh::Initialize()
{
	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

	int a = MV1GetMaterialNum(modelHandle);

	for (int i = 0; i < a; i++)
	{
		// ３Ｄモデルに含まれる０番目のマテリアルの描画ブレンドモードを DX_BLENDMODE_ADD に変更する
		MV1SetMaterialDrawBlendMode(modelHandle, i, DX_BLENDMODE_ADD);
		// マテリアルのブレンドパラメータを 128 に変更する
		MV1SetMaterialDrawBlendParam(modelHandle, i, 0);
	}
}

/// <summary>
/// 更新
/// </summary>
void FieldMesh::Update()
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
void FieldMesh::Draw()
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