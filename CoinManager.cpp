#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "Player.h"
#include "EffectManager.h"
#include "CoinManager.h"
#include "nlohmann/json.hpp"

/// <summary>
/// コンストラクタ
/// </summary>
CoinManager::CoinManager()
{
	tag = "coin";
}

/// <summary>
/// デストラクタ
/// </summary>
CoinManager::~CoinManager()
{
	coins.clear();
}

void CoinManager::Create()
{
	std::string modelPath = jsonData["modelPath"];

	modelHandle = MV1LoadModel(modelPath.c_str());

	for (auto& pos : jsonData["coin_list"])
	{
		coins.push_back(std::make_shared<CoinObject>());
		coins.back()->Load(modelHandle,
			VGet(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()));
	}
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="path"></param>
void CoinManager::Initialize()
{

	for (auto& coin : coins)
	{
		coin->Initialize();
	}

}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="player"></param>
/// <param name="cameraLookPos"></param>
void CoinManager::Update(const std::shared_ptr<Player>& player,
	std::shared_ptr<EffectManager>& effectManager, 
	const VECTOR& cameraLookPos)
{
	//vector型内の現在位置
	std::vector<std::shared_ptr<CoinObject>>::iterator it;

	for ( it = coins.begin(); it != coins.end();)
	{
		//playerと当たっていたら削除する
		if ((*it)->Update(effectManager,
			player->GetTopPos(), 
			player->GetBottomPos(),
			player->GetRadius()))
		{
			NotifyCoinPicked(coinValue);

			//コインを削除
			it = coins.erase(it);
			continue;
		}
		it++;
	}
	pos_addObject = cameraLookPos;

}

void CoinManager::Draw()
{
	for (auto& coin : coins)
	{
		coin->Draw();
	}
}

void CoinManager::Add()
{
	coins.push_back(std::make_shared<CoinObject>());
	coins.back()->Load(modelHandle,
		pos_addObject);
	coins.back()->Initialize();
}

/// <summary>
/// 登録されているオブザーバーに通知する
/// </summary>
/// <param name="amount コインの枚数"></param>
void CoinManager::NotifyCoinPicked(int amount)
{
	//リスト内の要素を一つずつ調査する
	for (auto it = observers.begin(); it != observers.end(); ) 
	{
		//lockでweak_ptrからshared_ptrへ一時的に変換
		if (auto obs = it->lock()) 
		{
			obs->OnCoinPicked(amount);
			++it;
		}
		else 
		{
			// すでに破棄されている observer を削除
			it = observers.erase(it);
		}
	}
}

/// <summary>
/// オブザーバーの解除
/// </summary>
/// <param name="observer"></param>
void CoinManager::RemoveObserver(std::shared_ptr<CoinObserver> observer)
{
	// 削除対象か判定するラムダ関数を用意
	auto shouldRemove = [&](const std::weak_ptr<CoinObserver>& weakObs) {
		// weak_ptrからshared_ptrを取得
		std::shared_ptr<CoinObserver> locked = weakObs.lock();

		// 生きていて、かつobserverと同じオブジェクトならtrue
		if (locked) {
			return locked == observer;
		}

		// 期限切れまたは違うオブジェクトならfalse
		return false;
		};

	// remove_ifを呼んで削除対象の要素を末尾に移動させる
	auto newEnd = std::remove_if(observers.begin(), observers.end(), shouldRemove);

	// 実際にvectorの末尾の削除対象部分を削除する
	observers.erase(newEnd, observers.end());
}

void CoinManager::Update(){}
