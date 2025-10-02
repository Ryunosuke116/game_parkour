#include "common.h"
#include <vector>
#include <memory>
#include "FinishCut.h"
#include "PadInput.h"


FinishCut::FinishCut() :
	finishHandle(-1),
	finishGraphPosX(-1),
	finishGraphPosY(-1),
	finishGraphTimer(-1.0f),
	isDrawFinish(false)
{
	tag = "png";
}

FinishCut::~FinishCut()
{
	DeleteGraph(finishHandle);
}

void FinishCut::Load(const nlohmann::json& jsonData)
{
	std::unordered_map<std::string, std::string> finishPath;

	for (auto& data : jsonData["finish"])
	{
		std::string path = data[0];     //HandlePath
		std::string name = data[1];     //path‚Ì–¼‘O

		finishPath[name] = path;
	}

	finishHandle = LoadGraph(finishPath.at("finish").c_str());
}

void FinishCut::Initialize()
{
	const float kInitFinishGraphTimer = 0.0f;
	const float kInitFinishGraphPosX = 600.0f;
	const float kInitFinishGraphPosY = 350.0f;

	isDrawFinish = false;
	finishGraphTimer = kInitFinishGraphTimer;
	finishGraphPosX = kInitFinishGraphPosX;
	finishGraphPosY = kInitFinishGraphPosY;
}

bool FinishCut::Update()
{
	const float kMaxFinishGraphTimer = 50.0f;

	if (isDrawFinish)
	{
		finishGraphTimer++;
		if (finishGraphTimer >= kMaxFinishGraphTimer)
		{
			return true;
		}
	}
	return false;
}

void FinishCut::Draw()
{
	if (isDrawFinish)
	{
		DrawGraph(finishGraphPosX, finishGraphPosY, finishHandle, TRUE);
	}
}