#include "common.h"
#include <vector>
#include <unordered_map>
#include "UI_controlManual.h"
#include "AnimTime.h"
#include "JsonManager.h"

UI_controlManual::UI_controlManual():
	BaseUI()
{
	stateNumber = -1;
	jsonTag = "controlManual";
}


UI_controlManual::~UI_controlManual()
{
	for (auto& uiHandle : uiHandles)
	{
		DeleteGraph(uiHandle.second);
	}
}


void UI_controlManual::Load(const nlohmann::json& jsonData)
{
	for (auto& data : jsonData["controlManual"])
	{
		std::string path = data[0];
		std::string name = data[1];

		uiHandles[name] = LoadGraph(path.c_str());
	}
}

void UI_controlManual::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

void UI_controlManual::Initialize()
{
	const int initX = 90;
	const int initY = 120;
	const int initStateNumber = -1;

	x = initX;
	y = initY;
	commandsBackPosX = 30;
	commandsBackPosY = 60;
	stateNumber = initStateNumber;
}

/// <summary>
/// playerの状態によって表示するUIを変更する
/// </summary>
void UI_controlManual::Update()
{
	draw_UIs.clear();

	draw_UIs.push_back(uiHandles.at("move_camera"));
	draw_UIs.push_back(uiHandles.at("reset_camera"));

	if ((!data.isHanging_now ||
		!data.isRunWall) &&
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("move"));
	}

	if (!data.isJumpAll && 
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("jump"));
	}

	if (!data.isUseRoll &&
		!data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("roll"));
	}
	
	if (data.isHanging_now)
	{
		draw_UIs.push_back(uiHandles.at("move_up"));
		draw_UIs.push_back(uiHandles.at("move_down"));
	}

	if (data.isRunWall)
	{
		draw_UIs.push_back(uiHandles.at("move_down"));
	}
}

void UI_controlManual::Draw()
{
	int alpha_Box = 100;

	int draw_y = y;

	DrawGraph(commandsBackPosX, commandsBackPosY, uiHandles.at("commandBack"),TRUE);

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

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void UI_controlManual::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void UI_controlManual::ResultInitialize()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void UI_controlManual::ResultUpdate()
{
	//処理なし
}