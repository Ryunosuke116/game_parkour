#include "Common.h"
#include <string>
#include "GameTimer.h"
#include "DebugDrawer.h"
#include "JsonManager.h"
#include "Calculation.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameTimer::GameTimer() :
	BaseUI(),
	colonHandle(-1),
	colonPosX(-1),
	redBright(-1),
	greenBright(-1),
	blueBright(-1),
	addBright(-1),
	time(-1),
	setTime(-1),
	secTime(-1),
	minTime(-1),
	numPosX(-1.0f),
	numPosY(-1.0f),
	numWidth(-1.0f),
	numHeight(-1.0f),
	isUpdateMin(false),
	countNumberSec(""),
	countNumberMin("")
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
	const int kXNum = 10;
	const int kYNum = 10;
	const int kAllNum = 1;
	const int kXSize = 480;
	const int kYSize = 500;

	std::string path = jsonData["coin"][2][0];
	std::string colonPath = jsonData["coin"][3][0];

	LoadDivGraph(path.c_str(),
		kXNum, kYNum, kAllNum, kXSize, kYSize, numberHandle);
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
	const float initNumPosX		= 640.0;
	const float initNumPosY		= -290.0f;
	const float initColonPosX	= 765.0f;
	const float initNumWidth	= 100.0f;
	const float initHeight		= 100.0f;
	const int	kInitTime		= 0;
	const int	kInitSec		= 0;
	const int	kInitMin		= 3;
	const int	kInitAddBright	= -3;

	time			= kInitTime;
	setTime			= GetNowCount();
	secTime			= kInitSec;
	minTime			= kInitMin;
	numPosX			= initNumPosX;
	numPosY			= initNumPosY;
	colonPosX		= initColonPosX;
	numWidth		= initNumWidth;
	numHeight		= initHeight;
	isUpdateMin		= false;
	countNumberSec	= "";

	redBright	= kMaxBright;
	greenBright = kMaxBright;
	blueBright	= kMaxBright;
	addBright	= -3;
}

/// <summary>
/// 更新処理
/// </summary>
void GameTimer::Update()
{
	const int kReverseMul		= -1;
	const int kSubMin			= 1;
	const int kMaxSec			= 60;
	const float kTargetNumPosY	= 30;
	const float kLeapSpeed		= 0.1f;

	time = GetNowCount() - setTime;

	int elapsedSec = time / 1000;
	int elapsedMin = elapsedSec / 60;
	elapsedSec -= elapsedMin * 60;

	secTime = kMaxSec - elapsedSec;

	//60の時に分を減らす
	if (!isUpdateMin &&
		secTime == kMaxSec - kSubMin)
	{
		minTime = minTime - kSubMin;

		isUpdateMin = true;
	}

	secTime = TimeForciblyZero(kMaxSec);

	isUpdateMin = IsUpdateMin();

	countNumberSec = CreateCountNumber(secTime);
	countNumberMin = CreateCountNumber(minTime);

	numPosY = Calculation::Leap(numPosY, kTargetNumPosY, kLeapSpeed);

	#if defined(NDEBUG)

	#else
	//----------------------------------//
	// デバッグ用
	//----------------------------------//
		if (CheckHitKey(KEY_INPUT_9))
		{
			secTime = 0;
			minTime = 0;
		}
	#endif
		//残り1分を切ったらタイマーUIを点滅させる
		if (minTime <= 0)
		{
			greenBright += addBright;
			blueBright	+= addBright;

			//上下限に達した場合、加算値を反転させる
			if (greenBright <= kMinBright ||
				greenBright >= kMaxBright)
			{
				addBright *= kReverseMul;
			}
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

	SetDrawBright(redBright, greenBright, blueBright);

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

	SetDrawBright(kMaxBright, kMaxBright, kMaxBright);
}


std::string GameTimer::CreateCountNumber(const int time)
{
	std::string countNumber = "";
	const int kCharacterCount = 1;
	const int kFirstCount = 0;

	countNumber = std::to_string(time);

	if (countNumber.length() == kCharacterCount)
	{
		countNumber.insert(kFirstCount, "0");
		return countNumber;
	}

	return countNumber;
}

bool GameTimer::IsUpdateMin()
{
	const int count = 59;

	return secTime == count ? true : false;
}

int GameTimer::TimeForciblyZero(const int maxSec)
{
	int resetTime = 0;

	int resultTime = secTime == maxSec ? resetTime : secTime;

	return resultTime;
}

bool GameTimer::IsFinish()
{
	if (secTime == 0 &&
		minTime == 0)
	{
		return true;
	}
	return false;
}