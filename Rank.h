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

	int rankPosX;
	int rankPosY;
	int rankWidth;
	int rankHeight;
	int speechBubblePosX;
	int speechBubblePosY;
	int speechBubbleWidth;
	int speechBubbleHeight;
	int rankScale;
	int addScaling;
};

