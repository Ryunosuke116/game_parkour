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

}

void BackGround::Update()
{

}

void BackGround::Draw()
{
	const int backGroundX = 0;
	const int backGroundY = 0;
	const int startButtonX = 300;
	const int startButtonY = 650;

	DrawGraph(backGroundX, backGroundY, backGroundHandle, TRUE);

	DrawGraph(startButtonX, startButtonY, startButtonHandle, true);
}