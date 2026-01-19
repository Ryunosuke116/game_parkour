#include <iostream>
#include "RankScoreUpdater.h"
#include "RankScoreUi.h"

RankScoreUpdater::RankScoreUpdater():
	rankUpCount(-1),
	isChangeRank(false)
{
	//処理なし
}

RankScoreUpdater::~RankScoreUpdater()
{
	//処理なし
}

void RankScoreUpdater::Initialize()
{
	rankUpCount		= 0;
	isChangeRank	= false;
}

void RankScoreUpdater::Update(
	float& rankWidth,
	float& rankHeight,
	float& drawRankRate,
	std::string& rankHandleKey)
{
	const float kAddRankCount	= 0.15f;		//ゲージの上昇値
	const float kSubRankCount	= 0.001f;
	const float kMinSize		= 150.0f;
	const float kAddSize		= 30.0f;
	const float kSubSize		= 1.0f;

	isChangeRank				= false;

	//カウント分ゲージが上昇する
	if (rankUpCount >= 1)
	{
		drawRankRate -= kAddRankCount * float(rankUpCount);
		rankUpCount = 0;
		rankWidth += kAddSize;
		rankHeight += kAddSize;
	}

	//ランクの値が段々減っていく
	//ランク描画のサイズも段々元に戻っていく
	//ランクがついていない場合は、rate更新を行わない
	else if(rankHandleKey != "")
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
		rankHandleKey != "")
	{
		RankDown(drawRankRate, rankHandleKey);
		isChangeRank = true;
	}
	else if (drawRankRate <= kMinDrawRankRate &&
		rankHandleKey != "SSS")
	{
		RankUp(drawRankRate, rankHandleKey);
		isChangeRank = true;
	}

	//レートが上下限を超えた場合、制限する
	if (drawRankRate >= kMaxDrawRankRate)
	{
		drawRankRate = kMaxDrawRankRate;
	}
	else if (drawRankRate <= kMinDrawRankRate)
	{
		drawRankRate = kMinDrawRankRate;
	}
}

void RankScoreUpdater::OnCoinPicked(int amount)
{
	rankUpCount += amount;
}

void RankScoreUpdater::RankUp(
	float& drawRankRate,
	std::string& rankHandleKey)
{
	if (rankHandleKey == "")
	{
		rankHandleKey = "D";
	}
	else if (rankHandleKey == "D")
	{
		rankHandleKey = "C";
	}
	else if (rankHandleKey == "C")
	{
		rankHandleKey = "B";
	}
	else if (rankHandleKey == "B")
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

	//スコアの進行度にを少し余裕を持たせて変更する
	drawRankRate += kChangeAfterAddValue;
}

void RankScoreUpdater::RankDown(
	float& drawRankRate,
	std::string& rankHandleKey)
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
	else if (rankHandleKey == "B")
	{
		rankHandleKey = "C";
	}
	else if (rankHandleKey == "C")
	{
		rankHandleKey = "D";
	}
	else if (rankHandleKey == "D")
	{
		rankHandleKey = "";
	}

	//スコアの進行度にを少し余裕を持たせて変更する
	drawRankRate -= kChangeAfterAddValue;
}