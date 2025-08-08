#include "GameTimer.h"
#include "DebugDrawer.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameTimer::GameTimer()
{

}

/// <summary>
/// デストラクタ
/// </summary>
GameTimer::~GameTimer()
{

}

/// <summary>
/// 初期化
/// </summary>
void GameTimer::Initialize()
{
	time = 0;
	setTime = GetNowCount();
	sec = 0;
	min = 0;
	msec = 0;
}

/// <summary>
/// 更新処理
/// </summary>
void GameTimer::Update()
{
	time = GetNowCount() - setTime;

	//秒,分、ミリ秒を計算
	sec = time / 1000;
	min = sec / 60;
	sec -= min * 60;

	// ミリ秒算出(１０ミリ秒単位っぽい)
	msec = (time - sec * 1000 - min * 60000) / 10;

	DebugDrawer::Instance().InformationInput_string_int("min %d\n", min);
	DebugDrawer::Instance().InformationInput_string_int("sec %d\n", sec);
	DebugDrawer::Instance().InformationInput_string_int("msec %d\n", msec);
}

/// <summary>
/// 描画
/// </summary>
/// <returns></returns>
bool GameTimer::Draw()
{
	return false;
}