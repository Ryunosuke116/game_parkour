#pragma once
#include "BaseUI.h"
#include "CoinObserver.h"

class UI_coin : public BaseUI, public CoinObserver
{
public:
	UI_coin();
	~UI_coin();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void OnCoinPicked(int amount)override { coinCount += amount; }

private:
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int coinCount;
	std::string num;

};

