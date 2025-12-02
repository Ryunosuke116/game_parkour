#include "common.h"
#include <unordered_map>
#include "CoinObserver.h"
#include "RankScoreUi.h"
#include "JsonManager.h"
#include "DebugDrawer.h"
#include "Calculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
RankScoreUi::RankScoreUi() :
	BaseUI(),
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
	const float kInitRankPosX = 1350.0f;
	const float kInitRankPosY = 300.0f;
	const float lkInitRankWidth = 150.0f;
	const float lkInitRankHeight = 150.0f;

	rankPosX = kInitRankPosX;
	rankPosY = kInitRankPosY;
	rankWidth = lkInitRankWidth;
	rankHeight = lkInitRankHeight;
	drawRankRate = 0.0f;
	rankUpCount = 0;

	rankHandleKey = "S";
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
		drawRankRate -= 1.0f;
		if (drawRankRate <= 0.0f)
		{
			drawRankRate = 0.0f;
		}
	}
#endif

	RankUpdate();

	DebugDrawer::GetInstance().InformationInputStringFloat("drawRankRate %f\n", drawRankRate);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void RankScoreUi::Draw()
{
	int drawHeight = int(Calculation::InterpolationCalc(drawRankRate, 417.0f, 180.0f));
	DebugDrawer::GetInstance().InformationInputStringInt("drawHeight %d\n", drawHeight);

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
}

void RankScoreUi::OnCoinPicked(int amount)
{
	rankUpCount += amount;
}

void RankScoreUi::RankUpdate()
{
	const float kAddRankCount = 0.2f;
	const float kSubRankCount = 0.001f;
	const float kMinSize = 150.0f;
	const float kAddSize = 30.0f;
	const float kSubSize = 1.0f;

	//カウント分ランクアップする
	if (rankUpCount >= 1)
	{
		drawRankRate -= kAddRankCount * float(rankUpCount);
		rankUpCount = 0;
		rankWidth += kAddSize;
		rankHeight += kAddSize;
	}
	//ランクの値が段々減っていく
	//ランク描画のサイズも段々元に戻っていく
	else
	{
		drawRankRate += kSubRankCount;
		rankWidth -= kSubSize;
		rankHeight -= kSubSize;

		if (rankWidth <= kMinSize)
		{
			rankWidth = kMinSize;
		}

		if (rankHeight <= kMinSize)
		{
			rankHeight = kMinSize;
		}
	}

	if (drawRankRate >= kMaxDrawRankRate &&
		rankHandleKey != "B")
	{
		RankDown();
	}
	else if (drawRankRate <= kMinDrawRankRate &&
			rankHandleKey != "SSS")
	{
		RankUp();
	}

	if (drawRankRate >= kMaxDrawRankRate)
	{
		drawRankRate = kMaxDrawRankRate;
	}
	else if (drawRankRate <= kMinDrawRankRate)
	{
		drawRankRate = kMinDrawRankRate;
	}
}

void RankScoreUi::RankUp()
{
	if (rankHandleKey == "B")
	{
		rankHandleKey = "A";
	}
	else if (rankHandleKey == "A")
	{
		rankHandleKey = "S";
	}
	else if (rankHandleKey == "S")
	{
		rankHandleKey = "SS";
	}
	else if (rankHandleKey == "SS")
	{
		rankHandleKey = "SSS";
	}

	drawRankRate += kChangeAfterAddValue;
}

void RankScoreUi::RankDown()
{
	if (rankHandleKey == "SSS")
	{
		rankHandleKey = "SS";
	}
	else if (rankHandleKey == "SS")
	{
		rankHandleKey = "S";
	}
	else if (rankHandleKey == "S")
	{
		rankHandleKey = "A";
	}
	else if (rankHandleKey == "A")
	{
		rankHandleKey = "B";
	}

	drawRankRate -= kChangeAfterAddValue;
}