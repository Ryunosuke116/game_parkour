#include "common.h"
#include  "nlohmann/json.hpp"
#include "Wall.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="jsonData"></param>
Wall::Wall(nlohmann::json jsonData)
{
	std::string path = jsonData["path"];

	modelHandle = MV1LoadModel(path.c_str());
	tag = "moveFloor_1";
}

/// <summary>
/// デストラクタ
/// </summary>
Wall::~Wall()
{

}

/// <summary>
/// 初期化
/// </summary>
void Wall::Initialize()
{
	pos_difference = VGet(0.0f, 0.0f, 0.0f);
	position = VGet(5.0f, 80.0f, 100.0f);

	MV1SetPosition(modelHandle, position);
	MV1SetScale(modelHandle, VGet(0.5f, 0.5f, 0.5f));

	// モデルの０番目のフレームのコリジョン情報を構築
	MV1SetupCollInfo(modelHandle, -1, 1, 1, 1);

	MV1SetRotationXYZ(modelHandle, VGet(90.0f * DX_PI_F / 180.0f, 45.0f * DX_PI_F / 180.0f, 0.0f));
}

/// <summary>
/// 更新
/// </summary>
void Wall::Update()
{
	//MV1SetPosition(modelHandle, position);

	MV1RefreshCollInfo(modelHandle, -1);
}

void Wall::Draw()
{
	MV1DrawModel(modelHandle);
}