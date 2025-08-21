#include "common.h"
#include <string>
#include "GameTimer.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameTimer::GameTimer():
	BaseUI(),
	isUpdateMin(false),
	countNumberSec("")
{
	jsonTag = "png";
}

/// <summary>
/// デストラクタ
/// </summary>
GameTimer::~GameTimer()
{

}

void GameTimer::Load(const nlohmann::json& jsonData)
{
	std::string path = jsonData["coin"][2][0];

	LoadDivGraph(path.c_str(),
		10, 10, 1, 32, 64, numberHandle);
}

/// <summary>
/// 初期化
/// </summary>
void GameTimer::Initialize()
{
	time = 0;
	setTime = GetNowCount();
	sec = 0;
	min = 1;
	msec = 0;
	positionX = 724;
	positionY = 30;
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

	//秒,分を計算
	int elapsedSec = time / 1000;
	int elapsedMin = elapsedSec / 60;
	elapsedSec -= elapsedMin * 60;

	sec = maxSec - elapsedSec;
	
	//60秒経過ごとに分タイマーを更新
	if (!isUpdateMin &&
		sec == (maxSec - 1))
	{
		min = min - subMin;

		isUpdateMin = true;
	}


	//60秒は00秒表示とする
	sec = TimeForciblyZero(maxSec);
	
	///分タイマーを更新できるようにするか
	isUpdateMin = IsUpdateMin();

	countNumberSec = CreateCountNumber(sec);
	countNumberMin = CreateCountNumber(min);

	DebugDrawer::Instance().InformationInput_string_int("min %d\n", min);
	DebugDrawer::Instance().InformationInput_string_int("sec %d\n", sec);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
void GameTimer::Draw()
{
	int num_x = positionX;
	const int addNumber_x = 32;

	for (char c : countNumberMin)
	{
		int digit = c - '0';
		DrawGraph(num_x, positionY, numberHandle[digit], TRUE);
		num_x += addNumber_x;
	}

	num_x += addNumber_x;

	for (char c : countNumberSec)
	{
		int digit = c - '0';
		DrawGraph(num_x, positionY, numberHandle[digit], TRUE);
		num_x += addNumber_x;
	}
}

std::string GameTimer::CreateCountNumber(const int time)
{
	std::string countNumber = "";
	const int characterCount = 1;
	const int firstCount = 0;

	countNumber = std::to_string(time);

	//一文字しか入ってない場合先頭に0を挿入する
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