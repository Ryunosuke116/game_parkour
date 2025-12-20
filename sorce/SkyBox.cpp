#include "Common.h"
#include "SkyBox.h"
#include "BaseObject.h"
#include "JsonManager.h"
#include "WorldSubSystem.h"
#include "PlayerManager.h"

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
	const VECTOR kModelScale = VGet(2.7f, 2.7f, 2.7f);

	Load(JsonManager::GetInstance().GetJsons("field"));
	MV1SetScale(modelHandle, kModelScale);
}

/// <summary>
/// 初期化
/// </summary>
void SkyBox::Initialize()
{
	const VECTOR kInitPos = VGet(0.0f, 0.0f, 0.0f);
	position = kInitPos;
	MV1SetPosition(modelHandle, position);
}

void SkyBox::Update()
{
	auto spPlayerManager = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>();

	position = spPlayerManager->GetPosition();
	MV1SetPosition(modelHandle, position);
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