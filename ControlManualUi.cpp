#include "Common.h"
#include <vector>
#include <unordered_map>
#include "ControlManualUi.h"
#include "AnimTime.h"
#include "JsonManager.h"
#include "Calculation.h"

ControlManualUi::ControlManualUi():
	BaseUI(),
	stateNumber(-1)
{
	jsonTag = "controlManual";
}


ControlManualUi::~ControlManualUi()
{
	for (auto& uiHandle : uiHandles)
	{
		DeleteGraph(uiHandle.second);
	}
}


void ControlManualUi::Load(const nlohmann::json& jsonData)
{
	for (auto& data : jsonData["controlManual"])
	{
		std::string path = data[0];
		std::string name = data[1];

		uiHandles[name] = LoadGraph(path.c_str());
	}
}

void ControlManualUi::Create()
{
	Load(JsonManager::GetInstance().GetJsons("png"));
}

void ControlManualUi::Initialize()
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
void ControlManualUi::Update()
{
	const float kLeapSpeed = 0.1f;
	const float targetManualPosX = 90.0f;
	const float targetCommandsBackPosX = 30.0f;

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

	manualUiPosX = Calculation::Leap(manualUiPosX, targetManualPosX, kLeapSpeed);
	commandsBackPosX = Calculation::Leap(commandsBackPosX, targetCommandsBackPosX, kLeapSpeed);
}

void ControlManualUi::Draw()
{
	const int kAddPosY = 40;

	float nowManualUiPosY = manualUiPosY;

	DrawGraphF(commandsBackPosX, commandsBackPosY, uiHandles.at("commandBack"),TRUE);

	for (auto& drawUi : drawUis)
	{
		DrawGraphF(manualUiPosX, nowManualUiPosY, drawUi, TRUE);
		nowManualUiPosY += kAddPosY;
	}
}

void ControlManualUi::OnChangeState(const PlayerData& playerData)
{
	data = playerData;
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void ControlManualUi::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void ControlManualUi::ResultInitialize()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void ControlManualUi::ResultUpdate()
{
	//処理なし
}