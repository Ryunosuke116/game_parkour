#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "EffectManager.h"
#include "CoinManager.h"
#include "nlohmann/json.hpp"
#include "WorldSubSystem.h"
#include "JsonManager.h"
#include "PlayerManager.h"

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
	resultUpdateCoins.clear();
	int a = MV1DeleteModel(modelHandle);
	observers.clear();
}

void CoinManager::Create()
{
	auto self = shared_from_this();

	nlohmann::json data = JsonManager::GetInstance().GetJsons("coin");
	std::string modelPath = data["modelPath"];

	modelHandle = MV1LoadModel(modelPath.c_str());

	for (auto& pos : data["coin_list"])
	{
		coins.push_back(std::make_shared<CoinObject>());
		coins.back()->Load(modelHandle,
			VGet(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()));
	}

	WorldSubSystem::GetInstance().AddSubSystem<CoinManager>(self);
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


void CoinManager::Update()
{
	//vector型内の現在位置
	std::vector<std::shared_ptr<CoinObject>>::iterator it;

	for (it = coins.begin(); it != coins.end();)
	{
		//playerと当たっていたら削除する
		if ((*it)->Update(
			WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPlayer()->GetTopPos(),
			WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPlayer()->GetBottomPos(),
			WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>()->GetPlayer()->GetRadius()
			))
		{
			NotifyCoinPicked(coinValue);

			//コインを削除
			it = coins.erase(it);
			continue;
		}
		it++;
	}

	posAddObject = WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetLookPosition();
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
		posAddObject);
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

void CoinManager::ResultCreate(int coinCount)
{
	nlohmann::json data = JsonManager::GetInstance().GetJsons("coin");
	std::string modelPath = data["modelPath"];

	modelHandle = MV1LoadModel(modelPath.c_str());

	for (auto& pos : data["coin_list"])
	{
		coins.push_back(std::make_shared<CoinObject>());
		coins.back()->Load(modelHandle,
			VGet(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()));
	}
}

/// <summary>
/// リザルトシーン時の初期化処理
/// </summary>
void CoinManager::ResultInitialize()
{
	for (auto& coin : coins)
	{
		coin->ResultInitialize();
	}
	timer = 0.0f;
	nowCoinCount = 0;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void CoinManager::ResultUpdate()
{
	timer++;
	//タイマーが規定値を超えたら更新処理するコインを追加する
	if (timer >= maxAddResultUpdateCoinTimer)
	{
		resultUpdateCoins.push_back(coins.at(nowCoinCount));
		nowCoinCount++;
		timer = 0.0f;
	}

	for (auto& weakCoin : resultUpdateCoins)
	{
		auto coin = weakCoin.lock();
		coin->ResultUpdate();
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
