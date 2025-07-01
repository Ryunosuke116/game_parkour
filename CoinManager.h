#pragma once
#include "CoinObject.h"

class CoinManager
{
private:
	std::vector<CoinObject> coins;

public:
	void Initialize(const char* path);
	void Update();
	void Draw();
};

