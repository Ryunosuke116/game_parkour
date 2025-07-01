#include <iostream>
#include <vector>
#include "CoinManager.h"

void CoinManager::Initialize(const char *path)
{
	for (int i = 0; i < 10; i++)
	{
		coins.push_back(CoinObject(path, VGet(0.0f, 0.0f, 0.0f)));
	}
}

void CoinManager::Update()
{
	for (auto& coin : coins)
	{
		coin.Update();
	}
}

void CoinManager::Draw()
{
	for (auto& coin : coins)
	{
		coin.Draw();
	}
}