#pragma once
#include "BaseGameObjectManager.h"
#include "BaseUI.h"
#include "CoinUi.h"
#include "UI_controlManual.h"
#include "GameTimer.h"

class UIManager : public BaseGameObjectManager
{
public:
	UIManager();
	~UIManager();

	void Create()		override;
	void Initialize()	override;
	void Update()		override;
	void Draw()			override;
	void Add()			override{}
	void ResultCreate()override{}
	void ResultInitialize()override;
	void ResultUpdate()override;

	void ResultCreate(const int coinCount);
	void Add(std::shared_ptr<BaseUI> ui);
	void ResultAdd(std::shared_ptr<BaseUI> ui);

	std::shared_ptr<CoinUi> GetCoinUi()
	{
		for (auto& ui : uiList)
		{
			if (auto ui_coin = std::dynamic_pointer_cast<CoinUi>(ui))
			{
				return ui_coin;
			}
		}
		return nullptr;
	}

	std::shared_ptr<UI_controlManual> GetControlManualUi()
	{
		for (auto& ui : uiList)
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
		for (auto& ui : uiList)
		{
			if (auto ui_gameTimer = std::dynamic_pointer_cast<GameTimer>(ui))
			{
				return ui_gameTimer;
			}
		}
		return nullptr;
	}


private:
	std::vector<std::shared_ptr<BaseUI>> uiList;
};

