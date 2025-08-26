#include "common.h"
#include "SkyBox.h"
#include "BaseObject.h"

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
	MV1SetScale(modelHandle, VGet(2.7f, 2.7f, 2.7f));
}

/// <summary>
/// 初期化
/// </summary>
void SkyBox::Initialize()
{

}

void SkyBox::Update() {};

