#pragma once
#include "BaseUI.h"
#include "CoinObserver.h"

class UI_coin : public BaseUI, public CoinObserver
{
public:
	UI_coin();
	~UI_coin();

	void Create()override {}
	void Add()override{}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void OnCoinPicked(int amount)override { coinCount += amount; }
	void SetCoinPos(int set_x, int set_y) { x = set_x, y = set_y; }

private:
	int cross_x;
	int cross_y;
	int number_x;
	int number_y;
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int coinCount;
	std::string countNumber;

};

