#include "common.h"
#include "Field.h"
#include "JsonManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
Field::Field() : 
	BaseObject(),
	isPush(false),
	isPoly(false)
{
	jsonTag = "field";
}

/// <summary>
/// デストラクタ
/// </summary>
Field::~Field()
{
	MV1DeleteModel(bullHandle);
}

void Field::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["path"];
	std::string bullPath = jsonData["bullPath"];

	tag = "field";
	modelHandle = MV1LoadModel(path.c_str());
	bullHandle = MV1LoadModel(bullPath.c_str());
}

void Field::Create()
{
	const VECTOR modelScale = VGet(0.9f, 0.9f, 0.9f);
	Load(JsonManager::GetInstance().GetJsons("field"));

	MV1SetScale(modelHandle, modelScale);
	MV1SetScale(bullHandle, modelScale);
}

/// <summary>
/// 初期化
/// </summary>
void Field::Initialize()
{
	position = VGet(0, 0, 0);
	MV1SetPosition(modelHandle, position);
	MV1SetPosition(bullHandle, position);

	isPoly = false;
}

/// <summary>
/// 更新
/// </summary>
void Field::Update()
{

}

/// <summary>
/// 描画
/// </summary>
void Field::Draw()
{
	MV1DrawModel(modelHandle);
	MV1DrawModel(bullHandle);
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void Field::ResultCreate()
{
	Create();
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void Field::ResultInitialize()
{
	Initialize();
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void Field::ResultUpdate()
{
	//処理なし
}