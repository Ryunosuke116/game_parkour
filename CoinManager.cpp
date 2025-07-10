#include <iostream>
#include <vector>
#include <fstream>
#include <string>
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
void CoinManager::Initialize()
{
	std::ifstream ifs("Json/coin.Json");
	nlohmann::json j;
	ifs >> j;

	std::string modelPath = j["modelPath"];

	modelHandle = MV1LoadModel(modelPath.c_str());

	for (auto& pos : j["coin_list"])
	{
		//std::string key(1, i);
		coins.push_back(std::make_shared<CoinObject>(modelHandle,
			VGet(pos[0], pos[1], pos[2])));
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
	pos_addObject = player->GetPosition();

}

bool CoinManager::Draw()
{
	for (auto& coin : coins)
	{
		coin->Draw();
	}
	return true;
}

void CoinManager::Add()
{
	coins.push_back(std::make_shared<CoinObject>(modelHandle, pos_addObject));
}

void CoinManager::Update(){}

void CoinManager::Create(){}