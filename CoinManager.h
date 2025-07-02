#pragma once
#include "CoinObject.h"

class CoinManager
{
private:
	std::vector<std::shared_ptr<CoinObject>> coins;

public:

	CoinManager();
	~CoinManager();

	void Initialize(const char* path);
	void Update(const std::shared_ptr<Player>& player);
	bool Draw();

};

