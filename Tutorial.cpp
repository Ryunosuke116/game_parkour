#include "common.h"
#include <vector>
#include <memory>
#include "Tutorial.h"
#include "PadInput.h"


Tutorial::Tutorial():
	tutorialGraphPosX(-1),
	tutorialGraphPosY(-1),
	startGraphPosX(-1),
	startGraphPosY(-1),
	startGraphTimer(-1),
	isPushStart(false)
{
	tag = "tutorial";
}

Tutorial::~Tutorial()
{
	DeleteGraph(tutorialHandle);
	DeleteGraph(startHandle);
}

void Tutorial::Load(const nlohmann::json& jsonData)
{
	std::string tutorialPath = jsonData["tutorialPath"];
	std::string startPath = jsonData["startPath"];
	tutorialHandle = LoadGraph(tutorialPath.c_str());
	startHandle = LoadGraph(startPath.c_str());
}

void Tutorial::Initialize()
{
	isPushStart = false;
	startGraphTimer = 0.0f;
	tutorialGraphPosX = 0;
	tutorialGraphPosY = 0;
	startGraphPosX = 600;
	startGraphPosY = 350;
}

bool Tutorial::Update()
{
	if (PadInput::IsPushA() &&
		!isPushStart)
	{
		isPushStart = true;
	}

	if (isPushStart)
	{
		startGraphTimer++;
		if (startGraphTimer >= 50.0f)
		{
			return false;
		}
	}

	return true;
}

void Tutorial::Draw(const float& streamStartPictureTimer)
{
	if (streamStartPictureTimer >= 50.0f &&
		!isPushStart)
	{
		DrawGraph(tutorialGraphPosX, tutorialGraphPosY, tutorialHandle, TRUE);
	}

	if (isPushStart)
	{
		DrawGraph(startGraphPosX, startGraphPosY, startHandle, TRUE);
	}
}

