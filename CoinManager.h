#pragma once
#include "Coin.h"
#include "BaseGameObjectManager.h"
#include "Player.h"
#include "CoinObserver.h"
#include "ObjectForTree.hpp"
#include <unordered_map>

class EffectManager;

class CoinManager :
	public BaseGameObjectManager,
	public std::enable_shared_from_this<CoinManager>
{
public:
	void AddObserver(std::shared_ptr<CoinObserver> observer) { observers.push_back(observer); }
	void RemoveObserver(std::shared_ptr<CoinObserver> observer);
	void NotifyCoinPicked(int amount);

	CoinManager();
	~CoinManager();

	void Create()override;
	void Initialize()override;
	void Update()override;
	void Draw()override;
	void Add()override;
	void ResultInitialize()override;
	void ResultUpdate()override;
	void ResultCreate()override;
	void ResultCreate(int coinCount);

	int GetModelHandle()const { return modelHandle; }
private:
	std::vector<std::shared_ptr<Coin>> activeCoins;
	std::unordered_map<int, std::shared_ptr<Coin>> umCoins;
	std::vector<std::weak_ptr<Coin>> resultUpdateCoins;
	std::vector<std::weak_ptr<CoinObserver>> observers;
	
	int modelHandle;
	int nowCoinCount;
	int addNumber;
	float timer;

	bool isAddResultUpdateCoin;				//更新するコインを追加するか
	
	static constexpr float maxAddResultUpdateCoinTimer = 30.0f;
	static constexpr int coinValue = 1;		//コインの価値

};

