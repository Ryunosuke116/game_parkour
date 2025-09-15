#include "common.h"
#include "SkyBox.h"
#include "BaseObject.h"
#include "JsonManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
SkyBox::SkyBox()
{
	jsonTag = "field";
}

/// <summary>
/// デストラクタ
/// </summary>
SkyBox::~SkyBox()
{

}

void SkyBox::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["skyBoxPath"];
	modelHandle = MV1LoadModel(path.c_str());
}

void SkyBox::Create()
{
	const VECTOR modelScale = VGet(2.7f, 2.7f, 2.7f);

	Load(JsonManager::GetInstance().GetJsons("field"));
	MV1SetScale(modelHandle, modelScale);
}

/// <summary>
/// 初期化
/// </summary>
void SkyBox::Initialize()
{
	//処理なし
}

void SkyBox::Update()
{
	//処理なし
};

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void SkyBox::ResultCreate()
{
	Create();
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void SkyBox::ResultInitialize()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void SkyBox::ResultUpdate()
{
	//処理なし
}