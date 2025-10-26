#include "common.h"
#include <unordered_map>
#include "RankScoreUi.h"
#include "JsonManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
RankScoreUi::RankScoreUi() :
	BaseUI(),
	rankPosX(-1),
	rankPosY(-1),
	rankWidth(-1),
	rankHeight(-1),
	rankHandleKey("")
{
	jsonTag = "png";
}

/// <summary>
/// デストラクタ
/// </summary>
RankScoreUi::~RankScoreUi()
{
	for (auto rankHandle : umRankHandles)
	{
		DeleteGraph(rankHandle.second);
	}
}

void RankScoreUi::Load(const nlohmann::json& jsonData)
{
	for (auto& data : jsonData["rank"])
	{
		std::string handlePath = data[0];     //HandlePath
		std::string name = data[1];				//pathの名前

		umRankHandles[name] = LoadGraph(handlePath.c_str());
	}
}

void RankScoreUi::Create()
{
	Load(JsonManager::GetInstance().GetJsons(jsonTag));
}

/// <summary>
/// 初期化
/// </summary>
void RankScoreUi::Initialize()
{
	const float kInitRankPosX = 1200.0f;
	const float kInitRankPosY = 150.0f;
	const float lkInitRankWidth = 300.0f;
	const float lkInitRankHeight = 300.0f;

	rankPosX = kInitRankPosX;
	rankPosY = kInitRankPosY;
	rankWidth = lkInitRankWidth;
	rankHeight = lkInitRankHeight;

	rankHandleKey = "A";
}

/// <summary>
/// 更新処理
/// </summary>
void RankScoreUi::Update()
{
	
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void RankScoreUi::Draw()
{
	DrawExtendGraphF(rankPosX,
		rankPosY,
		rankPosX + rankWidth,
		rankPosY + rankHeight,
		umRankHandles.at(rankHandleKey), TRUE);
}