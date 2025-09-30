#include "common.h"
#include <vector>
#include <memory>
#include "FinishCut.h"
#include "PadInput.h"


FinishCut::FinishCut() :
	finishHandle(-1),
	x_finishGraph(-1),
	y_finishGraph(-1),
	finishGraphTimer(-1),
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
	isDrawFinish = false;
	finishGraphTimer = 0.0f;
	x_finishGraph = 600;
	y_finishGraph = 350;
}

bool FinishCut::Update()
{
	const int maxFinishGraphTimer = 50.0f;

	if (isDrawFinish)
	{
		finishGraphTimer++;
		if (finishGraphTimer >= maxFinishGraphTimer)
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
		DrawGraph(x_finishGraph, y_finishGraph, finishHandle, TRUE);
	}
}