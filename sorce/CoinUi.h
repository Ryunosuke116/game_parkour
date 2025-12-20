#pragma once
#include "BaseUI.h"
#include "CoinObserver.h"

class CoinUi :
	public BaseUI,
	public CoinObserver
{
public:
	CoinUi();
	~CoinUi();

	void Create()override;
	void Add()override{}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void ResultCreate()override{}
	void ResultInitialize()override;
	void ResultUpdate()override;

	void ResultCreate(const int coinCount);
	void OnCoinPicked(int amount)override { coinCount += amount; }
private:
	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int coinCount;

	float coinPosX;
	float coinPosY;
	float crossPosX;
	float crossPosY;
	float numberPosX;
	float numberPosY;
	float coinWidth;
	float coinHeight;
	float crossWidth;
	float crossHeight;
	float numberWidth;
	float numberHeight;
	float addNumberX;
	std::string countNumber;
};

