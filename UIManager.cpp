#include "common.h"
#include <vector>
#include <memory>
#include "UIManager.h"

UIManager::UIManager() :
	BaseGameObjectManager()
{
	tag = "UI";
}

UIManager::~UIManager(){}

void UIManager::Create()
{
	Add(std::make_shared<UI_coin>());

}

void UIManager::Initialize()
{
	for (auto& UI : ui_list)
	{
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
	ui_list.back()->Load(jsonData[ui_list.back()->GetJsonTag()][0]);
}