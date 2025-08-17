#include "common.h"
#include <vector>
#include <memory>
#include "Tutorial.h"
#include "PadInput.h"


Tutorial::Tutorial():
	x_tutorialGraph(-1),
	y_tutorialGraph(-1),
	x_startGraph(-1),
	y_startGraph(-1),
	startGraph_timer(-1),
	isPush_start(false)
{
	tag = "tutorial";
}

Tutorial::~Tutorial()
{

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
	isPush_start = false;
	startGraph_timer = 0.0f;
	x_tutorialGraph = 200;
	y_tutorialGraph = 112;
	x_startGraph = 600;
	y_startGraph = 350;
}

bool Tutorial::Update()
{
	if (PadInput::IsPush_A() &&
		!isPush_start)
	{
		isPush_start = true;
	}

	if (isPush_start)
	{
		startGraph_timer++;
		if (startGraph_timer >= 50.0f)
		{
			return false;
		}
	}
	return true;
}

void Tutorial::Draw(const float& stream_startPicture_timer)
{
	if (stream_startPicture_timer >= 50.0f &&
		!isPush_start)
	{
		DrawGraph(x_tutorialGraph, y_tutorialGraph, tutorialHandle, TRUE);
	}

	if (isPush_start)
	{
		DrawGraph(x_startGraph, y_startGraph, startHandle, TRUE);
	}
}