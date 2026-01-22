#include "common.h"
#include <unordered_map>
#include <memory>
#include "RankScoreUi.h"
#include "JsonManager.h"
#include "DebugDrawer.h"
#include "Calculation.h"
#include "WorldSubSystem.h"
#include "RankScoreUpdater.h"

/// <summary>
/// コンストラクタ
/// </summary>
RankScoreUi::RankScoreUi() :
	BaseUI		(),
	rankPosX(-1),
	rankPosY(-1),
	rankWidth(-1),
	rankHeight(-1),
	rankUpCount(-1),
	rankHandleKey(""),
	drawRankRate(-1.0f)
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
		std::string handlePath	= data[0];     //HandlePath
		std::string name		= data[1];				//pathの名前

		umRankHandles[name] = LoadGraph(handlePath.c_str());
	}

	for (auto& data : jsonData["speedBonus"])
	{
		std::string handlePath	= data[0];
		std::string name		= data[1];

		umSpeedBonus[name] = LoadGraph(handlePath.c_str());
	}
}

void RankScoreUi::Create()
{
	Load(JsonManager::GetInstance().GetJsons(jsonTag));
	auto self = shared_from_this();

	//サブシステムに追加
	WorldSubSystem::GetInstance().AddSubSystem<RankScoreUi>(self);

	rankScoreUpdater = std::make_shared<RankScoreUpdater>();
}

/// <summary>
/// 初期化
/// </summary>
void RankScoreUi::Initialize()
{
	const float kInitRankPosX			= 1350.0f;
	const float kInitRankPosY			= 300.0f;
	const float kInitRankWidth			= 150.0f;
	const float kInitRankHeight			= 150.0f;
	const float kInitSpeedBonusPosX		= 1350.0f;
	const float kInitSpeedBonusPosY		= 450.0f;
	const float kInitSpeedBonusWidth	= 180.0f;
	const float kInitSpeedBonusHeight	= 100.0f;

	rankPosX		 = kInitRankPosX;
	rankPosY		 = kInitRankPosY;
	speedBonusPosX	 = kInitSpeedBonusPosX;
	speedBonusPosY	 = kInitSpeedBonusPosY;
	rankWidth		 = kInitRankWidth;
	rankHeight		 = kInitRankHeight;
	speedBonusWidth  = kInitSpeedBonusWidth;
	speedBonusHeight = kInitSpeedBonusHeight;
	drawRankRate	 = kResetRankRate;
	rankUpCount		 = 0;

	rankHandleKey	= "";
	rankScoreUpdater->Initialize();
}

/// <summary>
/// 更新処理
/// </summary>
void RankScoreUi::Update()
{
#if defined(NDEBUG)
	
#else	//デバッグ用
	if (CheckHitKey(KEY_INPUT_D))
	{
		drawRankRate += 1.0f;
		if (drawRankRate >= 1.0f)
		{
			drawRankRate = 1.0f;
		}
	}
	if (CheckHitKey(KEY_INPUT_A))
	{
		if (rankHandleKey != "")
		{
			drawRankRate -= 1.0f;
			if (drawRankRate <= 0.0f)
			{
				drawRankRate = 0.0f;
			}
		}
	}
#endif
	rankScoreUpdater->Update(rankWidth, rankHeight, drawRankRate, rankHandleKey);

	DebugDrawer::GetInstance().InformationInputStringFloat("drawRankRate %f\n", drawRankRate);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void RankScoreUi::Draw()
{
	const float kMaxHeight = 417.0f;
	const float kMinHeight = 180.0f;

	int drawHeight = int(Calculation::InterpolationCalc(drawRankRate, kMaxHeight, kMinHeight));
	DebugDrawer::GetInstance().InformationInputStringInt("drawHeight %d\n", drawHeight);

	if (rankHandleKey != "")
	{
		//ランクの外枠
		DrawExtendGraphF(
			rankPosX - rankWidth,
			rankPosY - rankHeight,
			rankPosX + rankWidth,
			rankPosY + rankHeight,
			umRankHandles.at(rankHandleKey), TRUE);

		//描画範囲を設定する
		SetDrawArea(0, drawHeight, 10000, 10000);

		//ランクの中の色の部分
		DrawExtendGraphF(
			rankPosX - rankWidth,
			rankPosY - rankHeight,
			rankPosX + rankWidth,
			rankPosY + rankHeight,
			umRankHandles.at(rankHandleKey + "_color"), TRUE);
	
		//描画範囲をリセット
		SetDrawArea(0, 0, 10000, 10000);

		if (rankHandleKey != "D")
		{
			DrawExtendGraphF(
				speedBonusPosX - speedBonusWidth,
				speedBonusPosY - speedBonusHeight,
				speedBonusPosX + speedBonusWidth,
				speedBonusPosY + speedBonusHeight,
				umSpeedBonus.at(rankHandleKey), TRUE);
		}
	}
}

bool RankScoreUi::GetIsChangeRank()const
{
	return rankScoreUpdater->GetIsChangeRank();
}