#pragma once
#include "CoinObserver.h"
#include "BaseObject.h"

class UI :public BaseObject, public CoinObserver
{
public:

	UI();
	~UI();
	void Initialize()override;
	void Update()override;
	bool Draw()override;


	void OnCoinPicked(int amount)override { coinCount += amount; }


private:
	int x;
	int y;
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int coinCount;

	std::string num;
};

