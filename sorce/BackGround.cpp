#include "Common.h"
#include "BackGround.h"
#include "JsonManager.h"

BackGround::BackGround():
	backGroundHandle	(-1),
	startButtonHandle	(-1),
	playMovieHandle		(-1),
	startButtonX		(-1),
	startButtonY		(-1),
	alpha				(-1),
	addAlpha			(-1),
	playMovieTime		(-1),
	setTime				(-1),
	sec					(-1),
	isPlayMovie			(false)
{
	
}

BackGround::~BackGround()
{
	PauseMovieToGraph(playMovieHandle);
	DeleteGraph(backGroundHandle);
	DeleteGraph(startButtonHandle);
	DeleteGraph(playMovieHandle);
}

void BackGround::Load(const nlohmann::json& jsonData)
{
	std::string backGroundPath	= jsonData["backGround"];
	std::string startButton		= jsonData["startButton"];
	std::string playMovie		= jsonData["playMovie"];

	backGroundHandle	= LoadGraph(backGroundPath.c_str());
	startButtonHandle	= LoadGraph(startButton.c_str());
	playMovieHandle		= LoadGraph(playMovie.c_str());
}

void BackGround::Create()
{
	Load(JsonManager::GetInstance().GetJsons("backGround"));
}

void BackGround::Initialize()
{
	const int kInitStartButtonX = 350;
	const int kInitStartButtonY = 650;
	const int kInitAlpha		= 254;
	const int kInitAddAlpha		= 2;
	
	startButtonX	= kInitStartButtonX;
	startButtonY	= kInitStartButtonY;
	alpha			= kInitAlpha;
	addAlpha		= kInitAddAlpha;
	playMovieTime	= 0;

	setTime		= GetNowCount();
	sec			= 0;
	isPlayMovie = false;
}

void BackGround::Update()
{
	const int kMaxAlpha			= 255;
	const int kMinAlpha			= 0;
	const int kAllMoviePlayTime = 170460;
	const int kDivTime			= 1000;

	alpha += addAlpha;

	if (alpha <= kMinAlpha || alpha >= kMaxAlpha)
	{
		addAlpha = -addAlpha;
	}

	playMovieTime = TellMovieToGraph(playMovieHandle);
	sec = (GetNowCount() - setTime) / kDivTime;
	
	if (TellMovieToGraph(playMovieHandle) >= kAllMoviePlayTime ||
		(playMovieTime == 0 && sec >= kCanPlayMovieSec))
	{
		SeekMovieToGraph(playMovieHandle, 0);
		PlayMovieToGraph(playMovieHandle);
	}

}

void BackGround::Draw()
{
	const int backGroundX = 0;
	const int backGroundY = 0;
	const int movieWidth  = 1600;
	const int movieHeight = 900;


	if (sec >= kCanPlayMovieSec)
	{
		DrawExtendGraph(0, 0, movieWidth, movieHeight, playMovieHandle, false);
	}
	else
	{
		DrawGraph(backGroundX, backGroundY, backGroundHandle, TRUE);
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(startButtonX, startButtonY, startButtonHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}