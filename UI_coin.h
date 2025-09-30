#pragma once
#include "BaseUI.h"
#include "CoinObserver.h"

class UI_coin :
	public BaseUI,
	public CoinObserver
{
public:
	UI_coin();
	~UI_coin();

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
	void SetCoinPos(int set_x, int set_y) { x = set_x, y = set_y; }

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

