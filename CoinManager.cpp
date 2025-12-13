#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "CoinManager.h"
#include "nlohmann/json.hpp"
#include "WorldSubSystem.h"
#include "JsonManager.h"
#include "PlayerManager.h"
#include "GameInstanceSubSystem.h"
#include "SoundPlayer.h"
#include "Liner8TreeManager.hpp"
#include "ObjectForTree.hpp"
#include "Cell.hpp"
#include "DebugDrawer.h"
#include "Coin.h"
#include "Player.h"

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
	umCoins.clear();
	resultUpdateCoins.clear();
	MV1DeleteModel(modelHandle);
	observers.clear();
}


void CoinManager::Create()
{
	auto self = shared_from_this();

	nlohmann::json data = JsonManager::GetInstance().GetJsons("coin");
	std::string modelPath = data["modelPath"];

	modelHandle = MV1LoadModel(modelPath.c_str());
	int i = 0;

	for (auto& pos : data["coin_list"])
	{
		umCoins[i] = std::make_shared<Coin>();
		umCoins.at(i)->Load(
			i,
			modelHandle,
			VGet(pos[0].get<float>(), pos[1].get<float>(), pos[2].get<float>()));
		i++;
	}

	WorldSubSystem::GetInstance().AddSubSystem<CoinManager>(self);
}

/// <summary>
/// 初期化
/// </summary>
/// <param name="path"></param>
void CoinManager::Initialize()
{
	auto L8TreeManager = WorldSubSystem::GetInstance().GetSubSystem<Liner8TreeManager<Coin>>();
	const int kInitAddNumber = 1000;

	for (auto& coin : umCoins)
	{
		coin.second->Initialize();

		//OFTに登録する
		std::shared_ptr<ObjectForTree<Coin>> OFT =
			std::make_shared<ObjectForTree<Coin>>();

		OFT->objectPointer = coin.second;

		//空間に登録
		L8TreeManager->Regist(
			coin.second->GetBoundsMin(),
			coin.second->GetBoundsMax(),
			OFT);
	}

	addNumber = kInitAddNumber;
}

/// <summary>
/// 更新処理
/// </summary>
void CoinManager::Update()
{
	const auto& L8TreeManager = WorldSubSystem::GetInstance().GetSubSystem<Liner8TreeManager<Coin>>();
	const auto& spPlayerManager = WorldSubSystem::GetInstance().GetSubSystem<PlayerManager>();

	//プレイヤーがどの空間にいるか調べる
	uint32_t playerSpaceNumber = L8TreeManager->GetMortonNumber(
		spPlayerManager->GetPlayer()->GetAABB().min,
		spPlayerManager->GetPlayer()->GetAABB().max);

	//playerのいる空間を検索
	std::shared_ptr<Cell<Coin>> cell = L8TreeManager->GetCell(playerSpaceNumber);

	//衝突する可能性のあるオブジェクトを走査する
	std::vector<std::shared_ptr<ObjectForTree<Coin>>> coinList;
	L8TreeManager->GetAllCollisionList(coinList, playerSpaceNumber);

	for (auto it = coinList.begin(); it != coinList.end();)
	{
		std::shared_ptr<Coin> coin = (*it)->objectPointer.lock();

		//中身がnullの場合次へ
		if (!coin)
		{
			it++;
			continue;
		}
		DebugDrawer::GetInstance().InformationInputAABB(coin->GetBoundsMin(), coin->GetBoundsMax(), GetColor(255, 0, 0));

		//playerと当たっていたら削除する
		if (coin->IsHitPlayer(spPlayerManager->GetPlayer()->GetTopPos(),
			spPlayerManager->GetPlayer()->GetBottomPos(),
			spPlayerManager->GetPlayer()->GetRadius()))
		{
			std::shared_ptr<Cell<Coin>> cell = (*it)->cellPointer.lock();

			//空間のobjectリストからも削除する
			if (cell)
			{
				std::shared_ptr<ObjectForTree<Coin>> spOFT = (*it);		//shared型のOFT
				auto& objectList = cell->GetObjectList();				//空間内のobjectリスト

				for (auto objectIt = objectList.begin(); objectIt != objectList.end(); objectIt++)
				{
					if (objectIt->get() == spOFT.get())
					{
						cell->OnRemove(objectIt);
						break;
					}
				}
			}

			//コイン取得を通知
			NotifyCoinPicked(coinValue);

			//空間オブジェクトリストからコインを削除
			it = coinList.erase(it);

			continue;
		}

		it++;
	}

	//全コインの更新処理
	for (auto umCoin = umCoins.begin(); umCoin != umCoins.end();)
	{
		umCoin->second->Update();

		//削除してよければリストから削除
		if (umCoin->second->GetIsDelete())
		{
			umCoin = umCoins.erase(umCoin);
			continue;
		}
		umCoin++;
	}

	posAddObject = WorldSubSystem::GetInstance().GetSubSystem<Camera>()->GetScreenCenterPosition();
	DebugDrawer::GetInstance().InformationInputStringInt("playerSpaceNumber %d", playerSpaceNumber);
}

void CoinManager::Draw()
{
	for (auto& coin : umCoins)
	{
		coin.second->Draw();
	}
}

/// <summary>
/// オブジェクトの追加
///LayOutクラスで生成する前提
/// </summary>
void CoinManager::Add()
{
	umCoins[addNumber] = std::make_shared<Coin>();
	umCoins.at(addNumber)->Load(addNumber,
		modelHandle,
		posAddObject);

	umCoins.at(addNumber)->Initialize();

	addNumber++;
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
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void CoinManager::ResultCreate()
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の生成
/// </summary>
/// <param name="coinCount"></param>
void CoinManager::ResultCreate(int coinCount)
{
	//処理なし
}

/// <summary>
/// リザルトシーン時の初期化処理
/// </summary>
void CoinManager::ResultInitialize()
{
	const float kInitTimer = 0.0f;
	const int kInitNowCoinCount = 0;

	for (auto& coin : umCoins)
	{
		coin.second->ResultInitialize();
	}
	timer = kInitTimer;
	nowCoinCount = kInitNowCoinCount;
}

/// <summary>
/// リザルトシーン時の更新処理
/// </summary>
void CoinManager::ResultUpdate()
{
	const size_t maxCoinCount = umCoins.size();
	const float kResetTimer = 0.0f;
	auto soundPlayer = GameInstanceSubSystem::GetInstance().GetSubSystem<SoundPlayer>().lock();

	timer++;
	//タイマーが規定値を超えたら更新処理するコインを追加する
	if (timer >= maxAddResultUpdateCoinTimer)
	{
		if (nowCoinCount < maxCoinCount)
		{
			nowCoinCount++;
			soundPlayer->Play("coinGet");
		}
		timer = kResetTimer;
	}

	for (int i = 0; i < nowCoinCount; i++)
	{
		umCoins.at(i)->ResultUpdate();
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
