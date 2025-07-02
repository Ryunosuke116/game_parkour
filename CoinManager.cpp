#include <iostream>
#include <vector>
#include <fstream>
#include "CoinManager.h"
#include "Json.h"
#include "nlohmann/json.hpp"


void CoinManager::Initialize(const char *path)
{
	//JsonFile::UnInitialize();
	//JsonFile::Initialize("Json/coin.json");
	//std::string positionPath = JsonFile::GetJson()["playerPath"];
	//std::ifstream ifs("Json/coin.json");
	//nlohmann::json j;
	//ifs >> j;

	//for (char i = 'a'; i < 'd'; i++)
	//{
	//	//positionPath = JsonFile::GetJson()[i];
	//	coins.push_back(CoinObject(path, VGet(j[i]["x"], j[i]["y"], j[i]["z"])));
	//}

	//for (int i = 0; i < 10; i++)
	//{
	//}
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