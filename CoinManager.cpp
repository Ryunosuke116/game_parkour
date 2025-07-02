#include <iostream>
#include <vector>
#include <fstream>
#include "Player.h"
#include "CoinManager.h"
#include "Json.h"
#include "nlohmann/json.hpp"

/// <summary>
/// コンストラクタ
/// </summary>
CoinManager::CoinManager()
{

}

/// <summary>
/// デストラクタ
/// </summary>
CoinManager::~CoinManager()
{

}

/// <summary>
/// 初期化
/// </summary>
/// <param name="path"></param>
void CoinManager::Initialize(const char *path)
{
	std::ifstream ifs("Json/coin.Json");
	nlohmann::json j;
	ifs >> j;

	for (char i = 'a'; i < 'd'; i++)
	{
		std::string key(1, i);
		if (j.contains(key))
		{
			coins.push_back(std::make_shared<CoinObject>(path, VGet(j[key]["x"], j[key]["y"], j[key]["z"])));
		}
	}

}

void CoinManager::Update(const std::shared_ptr<Player>& player)
{
	//vector型内の現在位置
	std::vector<std::shared_ptr<CoinObject>>::iterator it;

	for ( it = coins.begin(); it != coins.end();)
	{
		//playerと当たっていたら削除する
		if ((*it)->Update(player->GetTopPos(), player->GetBottomPos(), player->GetRadius()))
		{
			it = coins.erase(it);
			continue;
		}
		it++;
	}
}

bool CoinManager::Draw()
{
	for (auto& coin : coins)
	{
		coin->Draw();
	}
	return true;
}
