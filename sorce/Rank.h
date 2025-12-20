#pragma once
#include "BaseUI.h"
class Rank : public BaseUI
{
public:
	Rank();
	~Rank();

	void Add()override {}
	void Create()override{}
	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override{}
	void Update()override{}
	void Draw()override;
	void ResultCreate()override{}
	void ResultInitialize()override;
	void ResultUpdate()override;

	void ResultCreate(const int coinCount);
	void SetCoinCount(const int& set) { coinCount = set; }

private:
	int rankHandle;
	int speechBubbleHandle;
	int coinCount;

	float rankPosX;
	float rankPosY;
	float rankWidth;
	float rankHeight;
	float speechBubblePosX;
	float speechBubblePosY;
	float speechBubbleWidth;
	float speechBubbleHeight;
	int rankScale;
	int addScaling;
};

