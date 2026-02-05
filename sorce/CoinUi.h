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

	void Create()								override;
	void Add()									override  {}
	void Load(const nlohmann::json& jsonData)	override;
	void Initialize()							override;
	void Update()								override;
	void Draw()									override;
	void ResultCreate()							override  {}
	void ResultInitialize()						override;
	void ResultUpdate()							override;

	void ResultCreate(const int coinCount);
	void OnCoinPicked(int amount)override;

private:
	void PositionUp();

private:

	bool isUp;

	int coinHandle;
	int numberHandle[10];
	int crossHandle;
	int coinCount;

	float coinPosX;				//コインUIのX座標
	float coinPosY;				//コインUIのY座標
	float crossPosX;			//xUIのX座標
	float crossPosY;			//xUIのY座標
	float numberPosX;			//所持枚数UIのX座標
	float numberPosY;			//所持枚数UIのY座標
	float coinWidth;			//コインUIの横幅
	float coinHeight;			//コインUIの縦幅
	float crossWidth;			//xUIの横幅
	float crossHeight;			//xUIの縦幅
	float numberWidth;			//所持枚数UIの横幅
	float numberHeight;			//所持枚数UIの縦幅
	float addNumberX;			
	float uiPosMoveY;			//uiの座標の加算値
	float nowAddUiPosMoveY;		//現在縦軸に加算している値

	std::string countNumber;
};

