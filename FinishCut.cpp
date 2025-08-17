#include "common.h"
#include <vector>
#include <memory>
#include "FinishCut.h"
#include "PadInput.h"


FinishCut::FinishCut() :
	finishHandle(-1),
	x_finishGraph(-1),
	y_finishGraph(-1),
	finishGraph_timer(-1),
	isDraw_finish(false)
{
	tag = "png";
}

FinishCut::~FinishCut()
{

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
	isDraw_finish = false;
	finishGraph_timer = 0.0f;
	x_finishGraph = 600;
	y_finishGraph = 350;
}

bool FinishCut::Update()
{
	if (isDraw_finish)
	{
		finishGraph_timer++;
		if (finishGraph_timer >= 50.0f)
		{
			isDraw_finish = false;
			return true;
		}
	}
	return false;
}

void FinishCut::Draw()
{

	if (isDraw_finish)
	{
		DrawGraph(x_finishGraph, y_finishGraph, finishHandle, TRUE);
	}
}