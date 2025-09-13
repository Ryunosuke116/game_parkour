#include "common.h"
#include "BaseObject.h"
#include "FieldMesh.h"
#include "JsonManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="path"></param>
FieldMesh::FieldMesh():
	BaseObject(),
	isPoly(true),
	isPush(true)
{
	tag = "field";
	jsonTag = "field";
}

/// <summary>
/// デストラクタ
/// </summary>
FieldMesh::~FieldMesh()
{

}

void FieldMesh::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["meshPath"];
	modelHandle = MV1LoadModel(path.c_str());
	position = VGet(0, 0, 0);
	MV1SetScale(modelHandle, VGet(0.9f, 0.9f, 0.9f));
	MV1SetPosition(modelHandle, position);
	pos_difference = VGet(0.0f, 0.0f, 0.0f);
}

void FieldMesh::Create()
{
	Load(JsonManager::GetInstance().GetJsons(jsonTag));
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
			int a = MV1GetMaterialNum(modelHandle);

			if (!isPoly)
			{
				isPoly = true;
				for (int i = 0; i < a; i++)
				{
					// ３Ｄモデルに含まれる０番目のマテリアルの描画ブレンドモードを DX_BLENDMODE_ADD に変更する
					MV1SetMaterialDrawBlendMode(modelHandle, i, DX_BLENDMODE_ADD);
					// マテリアルのブレンドパラメータを 128 に変更する
					MV1SetMaterialDrawBlendParam(modelHandle, i, 255);
				}
			}
			else
			{
				isPoly = false;

				for (int i = 0; i < a; i++)
				{
					// ３Ｄモデルに含まれる０番目のマテリアルの描画ブレンドモードを DX_BLENDMODE_ADD に変更する
					MV1SetMaterialDrawBlendMode(modelHandle, i, DX_BLENDMODE_ADD);
					// マテリアルのブレンドパラメータを 128 に変更する
					MV1SetMaterialDrawBlendParam(modelHandle, i, 0);
				}
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
	//MV1SetWireFrameDrawFlag(modelHandle, FALSE);
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

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void FieldMesh::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void FieldMesh::ResultUpdate()
{
	//処理なし
}