#include "common.h"
#include "BackGround.h"
#include "JsonManager.h"

BackGround::BackGround()
{
	
}

BackGround::~BackGround()
{
	DeleteGraph(backGroundHandle);
	DeleteGraph(startButtonHandle);
}

void BackGround::Load(const nlohmann::json& jsonData)
{
	std::string backGroundPath = jsonData["backGround"];
	std::string startButton = jsonData["startButton"];

	backGroundHandle = LoadGraph(backGroundPath.c_str());
	startButtonHandle = LoadGraph(startButton.c_str());
}

void BackGround::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

void BackGround::Initialize()
{
	const int kInitStartButtonX = 350;
	const int kInitStartButtonY = 650;
	const int kInitAlpha = 254;
	const int kInitAddAlpha = 2;
	
	startButtonX = kInitStartButtonX;
	startButtonY = kInitStartButtonY;
	alpha = kInitAlpha;
	addAlpha = kInitAddAlpha;
}

void BackGround::Update()
{
	const int kMaxAlpha = 255;
	const int kMinAlpha = 0;

	alpha += addAlpha;

	if (alpha <= kMinAlpha || alpha >= kMaxAlpha)
	{
		addAlpha = -addAlpha;
	}
}

void BackGround::Draw()
{
	const int backGroundX = 0;
	const int backGroundY = 0;

	DrawGraph(backGroundX, backGroundY, backGroundHandle, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawGraph(startButtonX, startButtonY, startButtonHandle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}