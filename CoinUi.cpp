#include "common.h"
#include <algorithm>
#include "CoinUi.h"
#include "JsonManager.h"
#include "Calculation.h"

CoinUi::CoinUi() :
	BaseUI(),
	coinHandle(-1),
	crossHandle(-1),
	coinCount(-1),
	countNumber("")
{
	std::fill(
		std::begin(numberHandle),
		std::end(numberHandle),
		-1);

	jsonTag = "coin";
}

CoinUi::~CoinUi()
{
	for (auto& numberHandle : numberHandle)
	{
		DeleteGraph(numberHandle);
	}
	DeleteGraph(coinHandle);
	DeleteGraph(crossHandle);
}

void CoinUi::Load(const nlohmann::json& jsonData)
{
	std::unordered_map<std::string, std::string> uiPath;

	for (auto& data : jsonData["coin"])
	{
		std::string path = data[0];
		std::string name = data[1];

		uiPath[name] = path;
	}

	coinHandle = LoadGraph(uiPath.at("coin").c_str());
	crossHandle = LoadGraph(uiPath.at("cross").c_str());
	LoadDivGraph(uiPath.at("number").c_str(),
		10, 10, 1, 480, 500, numberHandle);
}

void CoinUi::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

/// <summary>
/// 初期化
/// </summary>
void CoinUi::Initialize()
{
	//描画位置
	const float InitCoinPosX = -480.0f;
	const float InitCoinPosY = 690.0f;
	const float InitCrossX = -345.0f;
	const float InitCrossY = 760.0f;
	const float InitNumberX = -290.0f;
	const float InitNumberY = 730.0f;
	const std::string initNumber = "00";

	coinPosX = InitCoinPosX;
	coinPosY = InitCoinPosY;
	crossPosX = InitCrossX;
	crossPosY = InitCrossY;
	numberPosX = InitNumberX;
	numberPosY = InitNumberY;

	coinWidth = 140.0f;
	coinHeight = 152.0f;
	crossWidth = 60.0f;
	crossHeight = 60.0f;
	numberWidth = 100.0f;
	numberHeight = 100.0f;
	addNumberX = 60.0f;

	countNumber = initNumber;
	coinCount = 0;
}

void CoinUi::Update()
{
	countNumber = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}

	const float targetCoinPosX = 20;
	const float targetCrossX = 155;
	const float targetNumberX = 210;

	coinPosX = Calculation::Leap(coinPosX, targetCoinPosX, 0.1f);
	crossPosX = Calculation::Leap(crossPosX, targetCrossX, 0.1f);
	numberPosX = Calculation::Leap(numberPosX, targetNumberX, 0.1f);
}

void CoinUi::Draw()
{
	//コインイラスト描画
	DrawExtendGraphF(coinPosX, coinPosY,
		coinPosX + coinWidth, coinPosY + coinHeight,
		coinHandle, TRUE);

	//cross描画
	DrawExtendGraphF(crossPosX, crossPosY,
		crossPosX + crossWidth, crossPosY + crossHeight,
		crossHandle, TRUE);

	float nowNumberPosX = numberPosX;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumber)
	{
		int digit = c - '0';
		DrawExtendGraphF(nowNumberPosX, numberPosY,
			nowNumberPosX + numberWidth, numberPosY + numberHeight,
			numberHandle[digit], TRUE);

		//文字の幅分ずらす
		nowNumberPosX += addNumberX;
	}
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void CoinUi::ResultCreate(const int coinCount)
{
	this->coinCount = coinCount;
	Load(JsonManager::GetInstance().GetJsons("png"));
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void CoinUi::ResultInitialize()
{
	const float InitCoinPosX = 1800.0f;
	const float InitCoinPosY = 630.0f;
	const float InitCrossX = 2005.0f;
	const float InitCrossY = 740.0f;
	const float InitNumberX = 2060.0f;
	const float InitNumberY = 690.0f;

	coinPosX = InitCoinPosX;
	coinPosY = InitCoinPosY;
	crossPosX = InitCrossX;
	crossPosY = InitCrossY;
	numberPosX = InitNumberX;
	numberPosY = InitNumberY;

	coinWidth = 200.0f;
	coinHeight = 212.0f;
	crossWidth = 76.0f;
	crossHeight = 76.0f;
	numberWidth = 150.0f;
	numberHeight = 150.0f;
	addNumberX = 120.0f;

	countNumber = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void CoinUi::ResultUpdate()
{
	const float kTargetCoinPosX = 960.0f;
	const float kTargetCrossX = 1165.0f;
	const float kTargetNumberX = 1220.0f;
	const float kLeapSpeed = 0.1f;

	coinPosX = Calculation::Leap(coinPosX, kTargetCoinPosX, kLeapSpeed);
	crossPosX = Calculation::Leap(crossPosX, kTargetCrossX, kLeapSpeed);
	numberPosX = Calculation::Leap(numberPosX, kTargetNumberX, kLeapSpeed);
}