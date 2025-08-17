#include "common.h"
#include <vector>
#include <memory>
#include "UIManager.h"

UIManager::UIManager() :
	BaseGameObjectManager()
{
	tag = "png";
}

UIManager::~UIManager(){}

void UIManager::Create()
{
	Add(std::make_shared<UI_coin>());
	Add(std::make_shared<UI_controlManual>());
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
	ui_list.push_back(ui);
	ui_list.back()->Load(jsonData[ui_list.back()->GetJsonTag()]);
}