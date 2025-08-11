#include "common.h"
#include "Field.h"

/// <summary>
/// コンストラクタ
/// </summary>
Field::Field() : BaseObject(),
	isPush(false),
	isPoly(false)
{

}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field()
{

}

void Field::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["path"];

	tag = "field";
	modelHandle = MV1LoadModel(path.c_str());
	MV1SetScale(modelHandle, VGet(0.9f, 0.9f, 0.9f));
}

/// <summary>
/// 初期化
/// </summary>
void Field::Initialize()
{
	position = VGet(0, 0, 0);
	MV1SetPosition(modelHandle, position);

	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);
	// モデルの０番目のフレームのコリジョン情報を構築
	//MV1SetupCollInfo(meshHandle, -1, 1, 1, 1);

	isPoly = false;

}

/// <summary>
/// 更新
/// </summary>
void Field::Update()
{
	/*if (CheckHitKey(KEY_INPUT_1))
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
	}*/

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
		//MV1SetWireFrameDrawFlag(meshHandle, TRUE);
	}
	else
	{
		MV1SetWireFrameDrawFlag(modelHandle, FALSE);
		//MV1SetWireFrameDrawFlag(meshHandle, FALSE);
	}

	MV1DrawModel(modelHandle);

}