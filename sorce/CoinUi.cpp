#include "Common.h"
#include <algorithm>
#include "CoinUi.h"
#include "JsonManager.h"
#include "Calculation.h"

CoinUi::CoinUi() :
	BaseUI(),
	isUp(false),
	isUpSecondDigit(false),
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
	nowAddUiPosMoveY(-1.0f),
	countNumber	(""),
	oldCountNumber("")
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

	coinHandle	= LoadGraph(uiPath.at("coin").c_str());
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
	const float InitCoinPosY		= 690.0f;
	const float InitCrossX			= 1945.0f;
	const float InitCrossY			= 760.0f;
	const float InitNumberX			= 1890.0f;
	const float InitNumberY			= 730.0f;
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

	countNumber			= initNumber;
	oldCountNumber		= initNumber;
	coinCount			= 0;
	nowAddUiPosMoveY	= 0.0f;
	isUp				= false;
}

void CoinUi::Update()
{
	const float targetCoinPosX	= 1170.0f;
	const float targetCrossX	= 1305.0f;
	const float targetNumberX	= 1360.0f;
	const float kLeapSpeed		= 0.1f;

	oldCountNumber = countNumber;

	countNumber = std::to_string(coinCount);

	//一文字しか入ってない場合先頭に0を挿入する
	if (countNumber.length() == 1)
	{
		countNumber.insert(0, "0");
	}

	IsAddNumberUiDataCheck();
	NumberDifferentCheck();

	PositionUp();

	coinPosX	= Calculation::Leap(coinPosX, targetCoinPosX, kLeapSpeed);
	crossPosX	= Calculation::Leap(crossPosX, targetCrossX, kLeapSpeed);
	numberPosX	= Calculation::Leap(numberPosX, targetNumberX, kLeapSpeed);

	for (auto& numberUiData : numberUiDatas)
	{
		numberUiData.posX = numberPosX;
	}
}

void CoinUi::Draw()
{
	//コインイラスト描画
	DrawExtendGraphF(
		coinPosX, 
		coinPosY,
		coinPosX + coinWidth,
		coinPosY + coinHeight,
		coinHandle,
		TRUE);

	//cross描画
	DrawExtendGraphF(
		crossPosX, 
		crossPosY,
		crossPosX + crossWidth,
		crossPosY + crossHeight,
		crossHandle, 
		TRUE);

	float newNumberPosX = numberPosX;

	int digitCount = 0;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumber)
	{
		//数字は何か
		int digit = c - '0';
		float newNumberPosY = numberUiDatas[digitCount].posY;

		if (numberUiDatas[digitCount].isUp)
		{
			newNumberPosY += uiPosMoveY;
		}

		DrawExtendGraphF(
			newNumberPosX,
			newNumberPosY,
			newNumberPosX	+ numberWidth,
			newNumberPosY	+ numberHeight,
			numberHandle[digit], 
			TRUE);

		//文字の幅分ずらす
		newNumberPosX += addNumberX;
		digitCount++;
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

	IsAddNumberUiDataCheck();
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

	coinPosX	= Calculation::Leap(coinPosX,	kTargetCoinPosX, kLeapSpeed);
	crossPosX	= Calculation::Leap(crossPosX,	kTargetCrossX,	kLeapSpeed);
	numberPosX	= Calculation::Leap(numberPosX, kTargetNumberX, kLeapSpeed);

	for (auto& numberUiData : numberUiDatas)
	{
		numberUiData.posX = numberPosX;
	}
}

void CoinUi::OnCoinPicked(int amount)
{ 
	coinCount	+= amount; 
	isUp		= true;
}

void CoinUi::PositionUp()
{
	const float kMaxPosY	= 30.0f;
	const float kAddValue	= 3.0f;

	if (isUp)
	{
		uiPosMoveY -= kAddValue;
		
		//一定値に到達したら元に数値を段々戻していく
		if (uiPosMoveY <= -kMaxPosY)
		{
			uiPosMoveY	= -kMaxPosY;
			isUp		= false;
		}
	}
	else
	{
		uiPosMoveY += kAddValue;
		
		//0以上になった場合動作を終了する
		if (uiPosMoveY > 0.0f)
		{
			uiPosMoveY = 0.0f;
			for (auto& numberUiData : numberUiDatas)
			{
				numberUiData.isUp = false;
			}
		}
	}
}

/// <summary>
/// 前フレームと現フレームの数字が違うか
/// </summary>
/// <param name="digitCount"></param>
void CoinUi::NumberDifferentCheck()
{
	for (int i = 0; i < numberUiDatas.size(); i++)
	{
		//桁が変動したときのエラー対策
		if (oldCountNumber.size() != countNumber.size())
		{
			numberUiDatas[i].isUp = true;
			continue;
		}
		else if (oldCountNumber.at(i) != countNumber.at(i))
		{
			numberUiDatas[i].isUp = true;
		}
	}
}

/// <summary>
/// 桁数を増やすかどうか
/// </summary>
void CoinUi::IsAddNumberUiDataCheck()
{
	//桁数が増えたら追加する
	while (numberUiDatas.size() != countNumber.size())
	{
		NumberUiData numberUiData;
		numberUiData.posX = numberPosX;
		numberUiData.posY = numberPosY;
		numberUiData.isUp = false;
		numberUiDatas.push_back(numberUiData);
	}
}