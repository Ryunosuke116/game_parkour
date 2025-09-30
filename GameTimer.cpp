#include "common.h"
#include <string>
#include "GameTimer.h"
#include "DebugDrawer.h"
#include "JsonManager.h"
#include "Calculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameTimer::GameTimer():
	BaseUI(),
	colonHandle(-1),
	colonPosX(-1),
	isUpdateMin(false),
	countNumberSec("")
{
	jsonTag = "coin";
}

/// <summary>
/// デストラクタ
/// </summary>
GameTimer::~GameTimer()
{
	for (auto& numberHandle : numberHandle)
	{
		DeleteGraph(numberHandle);
	}	
}

void GameTimer::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["coin"][2][0];
	std::string colonPath = jsonData["coin"][3][0];

	LoadDivGraph(path.c_str(),
		10, 10, 1, 480, 500, numberHandle);
	colonHandle = LoadGraph(colonPath.c_str());
}

void GameTimer::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

/// <summary>
/// 初期化
/// </summary>
void GameTimer::Initialize()
{
	const float initNumPosX = 640.0;
	const float initNumPosY = -290.0f;
	const float initColonPosX = 765.0f;
	const float initNumWidth = 100.0f;
	const float initHeight = 100.0f;

	time = 0;
	setTime = GetNowCount();
	sec = 0;
	min = 3;
	numPosX = initNumPosX;
	numPosY = initNumPosY;
	colonPosX = initColonPosX;
	numWidth = initNumWidth;
	numHeight = initHeight;
	isUpdateMin = false;
	countNumberSec = "";
}

/// <summary>
/// 更新処理
/// </summary>
void GameTimer::Update()
{
	const int subMin = 1;
	const int maxSec = 60;

	time = GetNowCount() - setTime;

	int elapsedSec = time / 1000;
	int elapsedMin = elapsedSec / 60;
	elapsedSec -= elapsedMin * 60;

	sec = maxSec - elapsedSec;

	//60の時に分を減らす
	if (!isUpdateMin &&
		sec == maxSec - 1)
	{
		min = min - subMin;

		isUpdateMin = true;
	}

	sec = TimeForciblyZero(maxSec);

	isUpdateMin = IsUpdateMin();

	countNumberSec = CreateCountNumber(sec);
	countNumberMin = CreateCountNumber(min);

	const float targetNumPosY = 30;

	numPosY = Calculation::Leap(numPosY, targetNumPosY, 0.1f);

	//----------------------------------//
	// デバッグ用
	//----------------------------------//

	if (CheckHitKey(KEY_INPUT_9))
	{
		sec = 0;
		min = 0;
	}
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void GameTimer::Draw()
{
	float numX = numPosX;
	const int addnumPosX = 70;
	const int addSpaceX = 40;

	//コイン所持数の桁数分、大きい桁から順に描画
	for (char c : countNumberMin)
	{
		int digit = c - '0';
		DrawExtendGraphF(numX,
			numPosY,
			numX + numWidth,
			numPosY + numHeight,
			numberHandle[digit], TRUE);

		//文字の幅分ずらす
		numX += addnumPosX;
	}

	DrawExtendGraphF(colonPosX,
		numPosY,
		colonPosX + numWidth,
		numPosY + numHeight,
		colonHandle, TRUE);
	numX += addSpaceX;

	for (char c : countNumberSec)
	{
		int digit = c - '0';
		DrawExtendGraphF(numX,
			numPosY,
			numX + numWidth,
			numPosY + numHeight,
			numberHandle[digit], TRUE);

		//文字の幅分ずらす
		numX += addnumPosX;
	}
}


std::string GameTimer::CreateCountNumber(const int time)
{
	std::string countNumber = "";
	const int characterCount = 1;
	const int firstCount = 0;

	countNumber = std::to_string(time);

	if (countNumber.length() == characterCount)
	{
		countNumber.insert(firstCount, "0");
		return countNumber;
	}

	return countNumber;
}

bool GameTimer::IsUpdateMin()
{
	const int count = 59;

	return sec == count ? true : false;
}

int GameTimer::TimeForciblyZero(const int maxSec)
{
	int resetTime = 0;

	int resultTime = sec == maxSec ? resetTime : sec;

	return resultTime;
}

bool GameTimer::IsFinish()
{
	if (sec == 0 &&
		min == 0)
	{
		return true;
	}
	return false;
}