#include "common.h"
#include <vector>
#include <memory>
#include "UIManager.h"
#include "JsonManager.h"

UIManager::UIManager() :
	BaseGameObjectManager()
{
	tag = "png";
}

UIManager::~UIManager()
{
	ui_list.clear();
}

void UIManager::Create()
{
	Add(std::make_shared<UI_coin>());
	Add(std::make_shared<UI_controlManual>());
	Add(std::make_shared<GameTimer>());
}

void UIManager::Initialize()
{
	const int coin_x = 30;
	const int coin_y = 700;

	for (auto& UI : ui_list)
	{
		if (auto ui_coin = std::dynamic_pointer_cast<UI_coin>(UI))
		{
			ui_coin->SetCoinPos(coin_x, coin_y);
		}
		UI->Initialize();
	}
}

void UIManager::Update()
{
	for (auto& UI : ui_list)
	{
		UI->Update();
	}
}

void UIManager::Draw()
{
	for (auto& UI : ui_list)
	{
		UI->Draw();
	}
}

void UIManager::Add(std::shared_ptr<BaseUI> ui)
{
	nlohmann::json data = JsonManager::GetInstance().GetJsons("png");
	ui_list.push_back(ui);
	ui_list.back()->Load(data[ui_list.back()->GetJsonTag()]);
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void UIManager::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void UIManager::ResultInitialize()
{
	const int coin_x = 30;
	const int coin_y = 700;

	for (auto& UI : ui_list)
	{
		if (auto ui_coin = std::dynamic_pointer_cast<UI_coin>(UI))
		{
			ui_coin->SetCoinPos(coin_x, coin_y);
		}
		UI->ResultInitialize();
	}
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void UIManager::ResultUpdate()
{
	for (auto& UI : ui_list)
	{
		UI->ResultUpdate();
	}
}