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
	startButtonX = 350;
	startButtonY = 650;
	alpha = 254;
	kAddAlpha = 2;
}

void BackGround::Update()
{
	alpha += kAddAlpha;

	if (alpha <= 0 || alpha >= 255)
	{
		kAddAlpha = -kAddAlpha;
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