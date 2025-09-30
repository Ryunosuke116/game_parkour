#include "common.h"
#include <vector>
#include <unordered_map>
#include "UI_controlManual.h"
#include "AnimTime.h"
#include "JsonManager.h"
#include "Calculation.h"

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
	const float initManualUiPosX = -350.0f;
	const float initManualUiPosY = 120.0f;
	const float initCommandsBackPosX = -290.0f;
	const float initCommandsBackPosY = 60.0f;
	const int initStateNumber = -1;

	manualUiPosX = initManualUiPosX;
	manualUiPosY = initManualUiPosY;
	commandsBackPosX = initCommandsBackPosX;
	commandsBackPosY = initCommandsBackPosY;
	stateNumber = initStateNumber;
}

/// <summary>
/// playerの状態によって表示するUIを変更する
/// </summary>
void UI_controlManual::Update()
{
	drawUis.clear();

	drawUis.push_back(uiHandles.at("move_camera"));
	drawUis.push_back(uiHandles.at("reset_camera"));

	if ((!data.isNowHanging ||
		!data.isRunWall) &&
		!data.isNowHanging)
	{
		drawUis.push_back(uiHandles.at("move"));
	}

	if (!data.isAllJump && 
		!data.isNowHanging)
	{
		drawUis.push_back(uiHandles.at("jump"));
	}

	if (!data.isUseRoll &&
		!data.isNowHanging)
	{
		drawUis.push_back(uiHandles.at("roll"));
	}
	
	if (data.isNowHanging)
	{
		drawUis.push_back(uiHandles.at("move_up"));
		drawUis.push_back(uiHandles.at("move_down"));
	}

	if (data.isRunWall)
	{
		drawUis.push_back(uiHandles.at("move_down"));
	}

	const float targetManualPosX = 90.0f;
	const float targetCommandsBackPosX = 30.0f;

	manualUiPosX = Calculation::Leap(manualUiPosX, targetManualPosX, 0.1f);
	commandsBackPosX = Calculation::Leap(commandsBackPosX, targetCommandsBackPosX, 0.1f);
}

void UI_controlManual::Draw()
{
	int alpha_Box = 100;
	const int kAddPosY = 40;

	int nowManualUiPosY = manualUiPosY;

	DrawGraphF(commandsBackPosX, commandsBackPosY, uiHandles.at("commandBack"),TRUE);

	for (auto& drawUi : drawUis)
	{
		DrawGraphF(manualUiPosX, nowManualUiPosY, drawUi, TRUE);
		nowManualUiPosY += kAddPosY;
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