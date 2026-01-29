#include "Common.h"
#include <algorithm>
#include "CoinUi.h"
#include "JsonManager.h"
#include "Calculation.h"

CoinUi::CoinUi() :
	BaseUI(),
	isUp(false),
	coinHandle	(-1),
	crossHandle	(-1),
	coinCount	(-1),
	coinPosX	(-1.0f),
	coinPosY	(-1.0f),
	crossPosX	(-1.0f),
	crossPosY	(-1.0f),
	numberPosX	(-1.0f),
	numberPosY	(-1.0f),
	coinWidth	(-1.0f),
	coinHeight	(-1.0f),
	crossWidth	(-1.0f),
	crossHeight	(-1.0f),
	numberWidth	(-1.0f),
	numberHeight(-1.0f),
	addNumberX	(-1.0f),
	uiPosMoveY	(-1.0f),
	countNumber	("")
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
	const int kXNum		= 10;
	const int kYNum		= 10;
	const int kAllNum	= 1;
	const int kXSize	= 480;
	const int kYSize	= 500;

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
		kXNum, kYNum, kAllNum, kXSize, kYSize, numberHandle);
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
	const float InitCoinPosX		= 2080.0f;
	const float InitCoinPosY		= 660.0f;
	const float InitCrossX			= 1945.0f;
	const float InitCrossY			= 730.0f;
	const float InitNumberX			= 1890.0f;
	const float InitNumberY			= 700.0f;
	const float kInitCoinWidth		= 140.0f;
	const float kInitCoinHeight		= 152.0f;
	const float kInitCrossWidth		= 60.0f;
	const float kInitCrossHeight	= 60.0f;
	const float kInitNumberWidth	= 100.0f;
	const float kInitNumberHeight	= 100.0f;
	const float kInitAddNumberX		= 60.0f;
	const std::string initNumber	= "00";

	coinPosX	= InitCoinPosX;
	coinPosY	= InitCoinPosY;
	crossPosX	= InitCrossX;
	crossPosY	= InitCrossY;
	numberPosX	= InitNumberX;
	numberPosY	= InitNumberY;

	coinWidth	 = kInitCoinWidth;
	coinHeight	 = kInitCoinHeight;
	crossWidth	 = kInitCrossWidth;
	crossHeight  = kInitCrossHeight;
	numberWidth  = kInitNumberWidth;
	numberHeight = kInitNumberHeight;
	addNumberX   = kInitAddNumberX;

	countNumber = initNumber;
	coinCount	= 0;
	isUp		= false;
}

void CoinUi::Update()
{
	const float targetCoinPosX	= 1110.0f;
	const float targetCrossX	= 1245.0f;
	const float targetNumberX	= 1300.0f;
	const float kLeapSpeed		= 0.1f;

	countNumber = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}

	PositionUp();

	coinPosX	= Calculation::Leap(coinPosX, targetCoinPosX, kLeapSpeed);
	crossPosX	= Calculation::Leap(crossPosX, targetCrossX, kLeapSpeed);
	numberPosX	= Calculation::Leap(numberPosX, targetNumberX, kLeapSpeed);
}

void CoinUi::Draw()
{
	//コインイラスト描画
	DrawExtendGraphF(coinPosX, 
		coinPosY + uiPosMoveY,
		coinPosX + coinWidth,
		coinPosY + coinHeight + uiPosMoveY,
		coinHandle, TRUE);

	//cross描画
	DrawExtendGraphF(crossPosX, 
		crossPosY + uiPosMoveY,
		crossPosX + crossWidth,
		crossPosY + crossHeight + uiPosMoveY,
		crossHandle, TRUE);

	float nowNumberPosX = numberPosX;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumber)
	{
		int digit = c - '0';
		DrawExtendGraphF(nowNumberPosX,
			numberPosY + uiPosMoveY,
			nowNumberPosX + numberWidth,
			numberPosY + numberHeight + uiPosMoveY,
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
	const float InitCoinPosX		= 1800.0f;
	const float InitCoinPosY		= 630.0f;
	const float InitCrossX			= 2005.0f;
	const float InitCrossY			= 740.0f;
	const float InitNumberX			= 2060.0f;
	const float InitNumberY			= 690.0f;
	const float kInitCoinWidth		= 200.0f;
	const float kInitCoinHeight		= 212.0f;
	const float kInitCrossWidth		= 76.0f;
	const float kInitCrossHeight	= 76.0f;
	const float kInitNumberWidth	= 150.0f;
	const float kInitNumberHeight	= 150.0f;
	const float kInitAddNumberX		= 120.0f;

	coinPosX	= InitCoinPosX;
	coinPosY	= InitCoinPosY;
	crossPosX	= InitCrossX;
	crossPosY	= InitCrossY;
	numberPosX	= InitNumberX;
	numberPosY	= InitNumberY;

	coinWidth		= kInitCoinHeight;
	coinHeight		= kInitCoinHeight;
	crossWidth		= kInitCrossWidth;
	crossHeight		= kInitCrossHeight;
	numberWidth		= kInitNumberWidth;
	numberHeight	= kInitNumberHeight;
	addNumberX		= kInitAddNumberX;

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
	const float kTargetCrossX	= 1165.0f;
	const float kTargetNumberX	= 1220.0f;
	const float kLeapSpeed		= 0.1f;

	coinPosX	= Calculation::Leap(coinPosX, kTargetCoinPosX, kLeapSpeed);
	crossPosX	= Calculation::Leap(crossPosX, kTargetCrossX, kLeapSpeed);
	numberPosX	= Calculation::Leap(numberPosX, kTargetNumberX, kLeapSpeed);
}

void CoinUi::OnCoinPicked(int amount)
{ 
	coinCount += amount; 
	isUp = true;
}

void CoinUi::PositionUp()
{
	if (isUp)
	{
		uiPosMoveY	= -30.0f;
		isUp		= false;
	}
	else
	{
		uiPosMoveY += 1.0f;
		
		if (uiPosMoveY > 0.0f)
		{
			uiPosMoveY = 0.0f;
		}
	}
}