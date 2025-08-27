#pragma once
#include "BaseGameObjectManager.h"
#include "BaseUI.h"
#include "UI_coin.h"
#include "UI_controlManual.h"
#include "GameTimer.h"

class UIManager : public BaseGameObjectManager
{
public:
	UIManager();
	~UIManager();

	void Create()		override;
	void Initialize()	override;
	void Update(ObjectMediator& objectMediator)		override;
	void Draw()			override;
	void Add()			override{}

	void Add(std::shared_ptr<BaseUI> ui);

	std::shared_ptr<UI_coin> GetUI_coin()
	{
		for (auto& ui : ui_list)
		{
			if (auto ui_coin = std::dynamic_pointer_cast<UI_coin>(ui))
			{
				return ui_coin;
			}
		}
		return nullptr;
	}

	std::shared_ptr<UI_controlManual> GetUI_controlManual()
	{
		for (auto& ui : ui_list)
		{
			if (auto ui_controlManual = std::dynamic_pointer_cast<UI_controlManual>(ui))
			{
				return ui_controlManual;
			}
		}
		return nullptr;
	}

	std::shared_ptr<GameTimer> GetGameTimer()
	{
		for (auto& ui : ui_list)
		{
			if (auto ui_gameTimer = std::dynamic_pointer_cast<GameTimer>(ui))
			{
				return ui_gameTimer;
			}
		}
		return nullptr;
	}


private:
	std::vector<std::shared_ptr<BaseUI>> ui_list;
};

