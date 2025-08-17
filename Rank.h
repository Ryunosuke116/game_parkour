#pragma once
#include "BaseUI.h"
class Rank : public BaseUI
{
public:
	Rank();
	~Rank();

	void Load(const nlohmann::json& jsonData)override;
	void Initialize()override;
	void Update()override;
	void Draw()override;

	void SetCoinCount(const int& set) { coinCount = set; }

private:
	int rankHandle;
	int coinCount;
};

