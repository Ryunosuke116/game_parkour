#pragma once
#include "BaseGameObjectManager.h"
#include "BaseUI.h"
#include "UI_coin.h"

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

private:
	std::vector<std::shared_ptr<BaseUI>> ui_list;
};

