#include "common.h"
#include <vector>
#include <unordered_map>
#include "UI_controlManual.h"
#include "AnimTime.h"

UI_controlManual::UI_controlManual():
	BaseUI()
{
	stateNumber = -1;
	jsonTag = "controlManual";
}


UI_controlManual::~UI_controlManual(){}


void UI_controlManual::Load(const nlohmann::json& jsonData)
{
	for (auto& data : jsonData)
	{
		std::string path = data[0];
		std::string name = data[1];

		uiHandles[name] = LoadGraph(path.c_str());
	}
}

void UI_controlManual::Initialize()
{
	x = 50;
	y = 30;
	stateNumber = -1;
}

void UI_controlManual::Update()
{
	draw_UIs.clear();

	draw_UIs.push_back(uiHandles.at("move_camera"));
	draw_UIs.push_back(uiHandles.at("reset_camera"));

	if ((!data.isHanging_now ||
		!data.isRun_wall) &&
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("move"));
	}

	if (!data.isJumpAll && 
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("jump"));
	}

	if (!data.isUse_Roll &&
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("roll"));
	}
	
	if (data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("move_up"));
		draw_UIs.push_back(uiHandles.at("move_down"));
	}

	if (data.isRun_wall)
	{
		draw_UIs.push_back(uiHandles.at("move_down"));
	}

}

void UI_controlManual::Draw()
{
	int draw_y = y;

	for (auto& draw_UI : draw_UIs)
	{
		DrawGraph(x, draw_y, draw_UI, TRUE);
		draw_y += 40;
	}
}

void UI_controlManual::OnChangeState(const PlayerData& playerData)
{
	data = playerData;
}