#include "Common.h"
#include <vector>
#include <memory>
#include "UIManager.h"
#include "JsonManager.h"
#include "Rank.h"
#include "UIButton.h"
#include "RankScoreUi.h"

UIManager::UIManager() :
	BaseGameObjectManager()
{
	tag = "png";
}

UIManager::~UIManager()
{
	uiList.clear();
}

void UIManager::Create()
{
	Add(std::make_shared<CoinUi>());
	Add(std::make_shared<ControlManualUi>());
	Add(std::make_shared<GameTimer>());

	for (auto& UI : uiList)
	{
		UI->Create();
	}
}

void UIManager::Initialize()
{
	for (auto& UI : uiList)
	{
		UI->Initialize();
	}
}

void UIManager::Update()
{
	for (auto& UI : uiList)
	{
		UI->Update();
	}
}

void UIManager::Draw()
{
	for (auto& UI : uiList)
	{
		UI->Draw();
	}
}

void UIManager::Add(std::shared_ptr<BaseUI> ui)
{
	uiList.push_back(ui);
}


/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void UIManager::ResultCreate(const int coinCount)
{
	ResultAdd(std::make_shared<Rank>());
	ResultAdd(std::make_shared<CoinUi>());
	ResultAdd(std::make_shared<UIButton>());

	for (auto& UI : uiList)
	{
		if (auto ui_coin = std::dynamic_pointer_cast<CoinUi>(UI))
		{
			ui_coin->ResultCreate(coinCount);
			continue;
		}
		if(auto ui_coin = std::dynamic_pointer_cast<Rank>(UI))
		{
			ui_coin->ResultCreate(coinCount);
			continue;
		}

		UI->ResultCreate();
	}
}

/// <summary>
/// リザルトシーン時の初期化
/// </summary>
void UIManager::ResultInitialize()
{
	for (auto& UI : uiList)
	{
		UI->ResultInitialize();
	}
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void UIManager::ResultUpdate()
{
	for (auto& UI : uiList)
	{
		UI->ResultUpdate();
	}
}

void UIManager::ResultAdd(std::shared_ptr<BaseUI> ui)
{
	uiList.push_back(ui);
}