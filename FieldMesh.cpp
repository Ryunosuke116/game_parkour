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
}

void FieldMesh::Create()
{
	const VECTOR kModelScale = VGet(0.9f, 0.9f, 0.9f);

	Load(JsonManager::GetInstance().GetJsons(jsonTag));
	position = VGet(0.0f, 0.0f, 0.0f);
	MV1SetScale(modelHandle, kModelScale);
	MV1SetPosition(modelHandle, position);
	differencePosition = VGet(0.0f, 0.0f, 0.0f);
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
	const int kMaxBlend = 255;
	const int kMinBlend = 0;

	if (CheckHitKey(KEY_INPUT_1))
	{
		if (!isPush)
		{
			int materialNum = MV1GetMaterialNum(modelHandle);

			if (!isPoly)
			{
				isPoly = true;
				ChangeBlendParam(materialNum, kMaxBlend);
			}
			else
			{
				isPoly = false;
				ChangeBlendParam(materialNum, kMinBlend);
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

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void FieldMesh::ResultCreate()
{
	Create();
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

void FieldMesh::ChangeBlendParam(const int materialNum, const int blendParam)
{
	for (int i = 0; i < materialNum; i++)
	{
		// ３Ｄモデルに含まれる０番目のマテリアルの描画ブレンドモードを DX_BLENDMODE_ADD に変更する
		MV1SetMaterialDrawBlendMode(modelHandle, i, DX_BLENDMODE_ADD);
		// マテリアルのブレンドパラメータを 128 に変更する
		MV1SetMaterialDrawBlendParam(modelHandle, i, blendParam);
	}
}